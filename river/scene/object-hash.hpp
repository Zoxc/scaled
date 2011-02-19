#pragma once
#include "../hash-table.hpp"
#include "../allocator.hpp"
#include "object-list.hpp"

namespace River
{
	template<class K, class V> class ObjectHash
	{
	private:
		/*
		 * TODO: These lists should be reference counted so they don't leak.
		 */
		class HashedObjectList:
			public ObjectList<V>
		{
		public:
			HashedObjectList(K key) : key(key) {}

			K key;
			Entry<HashedObjectList> hash_entry;
			HashedObjectList *hash_next;
		};

		class IntegerObjectHashFunctions:
			public HashTableFunctions<K, HashedObjectList *, ObjectHash *, StdLibAllocator>
		{
		public:
			static bool compare_key_value(K key, HashedObjectList *value)
			{
				return key == value->key;
			}

			static K get_key(HashedObjectList *value)
			{
				return value->key;
			}

			static HashedObjectList *get_value_next(HashedObjectList *value)
			{
				return value->hash_next;
			}

			static void set_value_next(HashedObjectList *value, HashedObjectList *next)
			{
				value->hash_next = next;
			}

			static bool valid_key(K key)
			{
				return key != 0;
			}

			static bool create_value()
			{
				return true;
			}

			static HashedObjectList *create_value(StdLibAllocator::Ref alloc_ref, ObjectHash *hash, K key)
			{
				HashedObjectList *list =  new HashedObjectList(key);
				hash->key_list.append(list);
				return list;
			}
		};

		HashTable<K, HashedObjectList *, ObjectHash *, IntegerObjectHashFunctions, StdLibAllocator> hash_table;

		typedef List<HashedObjectList, HashedObjectList, &HashedObjectList::hash_entry> ObjectLists;

		ObjectLists key_list;
	public:
		ObjectHash() : hash_table(1, this) {}

		void add(K key, V *object)
		{
			HashedObjectList *list = hash_table.get(key);
			list->append(object);
		}

		void remove(K key, V *object)
		{
			HashedObjectList *list = hash_table.get(key);
			list->remove(object);

			if(list->empty())
			{
				// TODO: Remove list from hashtable
			}
		}
		
		void render()
		{
			for(typename ObjectLists::Iterator i = key_list.begin(); i != key_list.end(); i++)
			{
				V::render_key(i().key);

				i().render();

				Scene::raise_errors();
			}
		}
	};
};
