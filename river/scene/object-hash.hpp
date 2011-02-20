#pragma once
#include "../hash-table.hpp"
#include "../allocator.hpp"
#include "../memory-pool.hpp"

namespace River
{
	class MemoryPool;

	template<class K, class V> class ObjectHash
	{
	public:
		class Functions:
			public HashTableFunctions<K, V *, bool, MemoryPool>
		{
		public:
			static bool compare_key_value(K key, V *value)
			{
				return key == value->key;
			}

			static K get_key(V *value)
			{
				return value->key;
			}

			static V *get_value_next(V *value)
			{
				return value->next;
			}

			static void set_value_next(V *value, V *next)
			{
				value->next = next;
			}

			static bool valid_key(K key)
			{
				return key != 0;
			}

			static bool create_value()
			{
				return true;
			}

			static V *create_value(MemoryPool alloc_ref, bool, K key)
			{
				V *list =  new (alloc_ref) V;
				list->key = key;
				return list;
			}
		};

		typedef HashTable<K, V *, bool, Functions, MemoryPool> HashTable;

		HashTable table;

		ObjectHash(MemoryPool &memory_pool) : table(1, false, memory_pool) {}
	};
};
