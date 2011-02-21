#pragma once
#include <cstdlib>
#include <cstring>
#include "allocator.hpp"

namespace River
{
	template<class K, class V, class S, class A> class HashTableFunctions
	{
		public:
			static size_t hash_key(K key)
			{
				return (size_t)key;
			}

			static bool valid_key(K key)
			{
				return key != 0;
			}

			static bool valid_value(V value)
			{
				return value != 0;
			}

			static V invalid_value()
			{
				return 0;
			}

			static bool create_value()
			{
				return false;
			}

			static V create_value(typename A::Ref alloc_ref, S storage, K key)
			{
				return 0;
			}
	};
	
	template<class K, class V, class S, class T, class A = StdLibAllocator> class HashTable
	{
		private:
			V *table;
			S storage;
			typename A::Storage alloc_ref;
			size_t mask;
			size_t entries;

			V store(V *table, size_t mask, K key, V value)
			{
				size_t index = T::hash_key(key) & mask;
				V entry = table[index];
				V tail = entry;

				while(T::valid_value(entry))
				{
					if(T::compare_key_value(key, entry))
					{
						if(T::valid_value(tail))
						{
							V next = T::get_value_next(entry);
							T::set_value_next(tail, value);
							T::set_value_next(value, next);
						}
						else
						{
							table[index] = value;
							T::set_value_next(value, T::invalid_value());
						}

						return entry;
					}

					tail = entry;
					entry = T::get_value_next(entry);
				}

				if(T::valid_value(tail))
					T::set_value_next(tail, value);
				else
					table[index] = value;

				T::set_value_next(value, T::invalid_value());

				return T::invalid_value();
			}

			void expand()
			{
				size_t old_size = this->mask + 1;
				size_t size = old_size << 1;
				size_t mask = size - 1;

				V *table = (V *)alloc_ref.alloc(size * sizeof(V));
				std::memset(table, 0, size * sizeof(V));

				V *end = this->table + old_size;

				for(V *slot = this->table; slot != end; ++slot)
				{
					V entry = *slot;

					while(T::valid_value(entry))
					{
						V next = T::get_value_next(entry);

						store(table, mask, T::get_key(entry), entry);

						entry = next;
					}
				}

				alloc_ref.free(this->table);

				this->mask = mask;
				this->table = table;
			}

			void increase()
			{
				entries++;

				if(entries > mask)
					expand();
			}

			void setup(size_t initial)
			{
				entries = 0;
				
				size_t size = 1 << initial;
				mask = size - 1;

				table = (V *)alloc_ref.alloc(size * sizeof(V));
				std::memset(table, 0, size * sizeof(V));
			}

		public:
			HashTable(size_t initial, S storage) : storage(storage), alloc_ref(A::Storage::def_ref())
			{
				setup(initial);
			}

			HashTable(size_t initial, S storage, typename A::Ref alloc_ref) : storage(storage), alloc_ref(alloc_ref)
			{
				setup(initial);
			}

			~HashTable()
			{
				alloc_ref.free(this->table);
			}

			V get(K key)
			{
				if(!T::valid_key(key))
					return 0;

				size_t index = T::hash_key(key) & mask;
				V entry = table[index];
				V tail = entry;

				while(T::valid_value(entry))
				{
					if(T::compare_key_value(key, entry))
						return entry;

					tail = entry;
					entry = T::get_value_next(entry);
				}

				if(T::create_value())
				{
					V value = T::create_value(alloc_ref, storage, key);

					if(tail)
						T::set_value_next(tail, value);
					else
						table[index] = value;

					T::set_value_next(value, T::invalid_value());

					increase();

					return value;
				}
				else
					return T::invalid_value();
			}

			V set(K key, V value)
			{
				V existing = store(table, mask, key, value);

				if(!T::valid_value(existing))
					increase();

				return existing;
			}
			
			V* get_table()
			{
				return table;
			}

			size_t get_size()
			{
				return mask + 1;
			}
			
			class Iterator
			{
			private:
				V *slot;
				V *end;
				V current;

			public:
				Iterator(V *start, V *end) : slot(start), end(end), current(T::invalid_value())
				{
					step();
				}

				Iterator(V *end) : slot(end), end(end), current(T::invalid_value()) {}

				void step()
				{
					if(T::valid_value(current))
					{
						current = T::get_value_next(current);

						if(T::valid_value(current))
							return;
					}

					do
					{
						if(slot == end)
							return;

						current = *slot++;
					}
					while(!T::valid_value(current));
				}
				
				bool operator ==(const Iterator &other) const
				{
					return (current == other.current) && (slot == other.slot);
				}
				
				bool operator !=(const Iterator &other) const
				{
					return (current != other.current) && (slot != other.slot);
				}
				
				V operator ++()
				{
					step();
					return current;
				}
				
				V operator ++(int)
				{
					V result = current;
					step();
					return result;
				}
				
				V operator*() const
				{
					return current;
				}
			};
			
			Iterator begin()
			{
				return Iterator(get_table(), get_table() + get_size());
			}

			Iterator end()
			{
				return Iterator(get_table() + get_size());
			}
	};
};
