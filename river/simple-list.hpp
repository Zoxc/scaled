#pragma once
#include <cassert>

namespace River
{
	template<class T> class SimpleEntry
	{
	public:
		SimpleEntry() : next(0) {}

		T *next;
	};

	template<class T, class E = T, SimpleEntry<E> E::*field = &E::entry> class SimpleList
	{
	public:
		SimpleList() : first(0), last(0) {}
		
		T *first;
		T *last;
		
		bool empty()
		{
			return first == 0;
		}

		void append(T *node)
		{
			assert(node != 0);
			
			(node->*field).next = 0;
			
			if(last != 0)
			{
				(last->*field).next = static_cast<E *>(node);
				last = node;
			}
			else
			{
				first = node;
				last = node;
			}
		}

		class Iterator
		{
		private:
			T *current;

		public:
			Iterator(T *start) : current(start) {}

			void step()
			{
				current = static_cast<T *>((current->*field).next);
			}
			
			bool operator ==(const Iterator &other) const
			{
				return current == other.current;
			}
			
			bool operator !=(const Iterator &other) const
			{
				return current != other.current;
			}
			
			T &operator ++()
			{
				step();
				return *current;
			}
			
			T &operator ++(int)
			{
				T *result = current;
				step();
				return *result;
			}
			
			T *operator*() const
			{
				return current;
			}

			T &operator ()() const
			{
				return *current;
			}
		};
		
		Iterator begin()
		{
			return Iterator(first);
		}

		Iterator end()
		{
			return Iterator(0);
		}
	};
};
