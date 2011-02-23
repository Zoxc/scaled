#pragma once
#include <stdint.h>
#include "../hash-table.hpp"
#include "../gles.hpp"
#include "../memory-pool.hpp"

namespace River
{
	class Layer;

	class LayerContext
	{
	public:
		class Entry
		{
			public:
				enum Type
				{
					GlyphContext,
					GradientContext
				};

				uint32_t entry_type;
				Entry *context_next;

				static GLfloat *buffer_coords(GLfloat *buffer, GLfloat x, GLfloat y, GLfloat x2, GLfloat y2);
				static GLshort *buffer_quad(GLshort *buffer, int x, int y, int width, int height);
			
				Entry(uint32_t entry_type) : entry_type(entry_type)
				{
				}
			
				virtual void render(Layer *layer) = 0;
		};

	private:
		class EntryFunctions:
			public HashTableFunctions<uint32_t, Entry *, bool, MemoryPool>
		{
		public:
			static bool compare_key_value(uint32_t key, Entry *value)
			{
				return key == value->entry_type;
			}

			static uint32_t get_key(Entry *value)
			{
				return value->entry_type;
			}

			static Entry *get_value_next(Entry *value)
			{
				return value->context_next;
			}

			static void set_value_next(Entry *value, Entry *next)
			{
				value->context_next = next;
			}

			static bool valid_key(uint32_t key)
			{
				return true;
			}
		};

		typedef HashTable<uint32_t, Entry *, bool, EntryFunctions, MemoryPool> EntryMap;
		
		EntryMap map;

	public:
		LayerContext(MemoryPool &memory_pool);

		MemoryPool &memory_pool;
		
		Entry *lookup(uint32_t entry_type);
		void store(Entry *content);

		Layer *render();
	};
};
