#pragma once
#include <stdint.h>
#include "../hash-table.hpp"
#include "../gles.hpp"
#include "../memory-pool.hpp"

namespace River
{
	class Layer;

	class ContentMeasurer;
	class ContentSerializer;

	class LayerCanvas
	{
	public:
		class Entry
		{
			public:
				enum Type
				{
					GlyphCanvas,
					GradientCanvas,
					ColoredImageCanvas
				};

				uint32_t entry_type;
				Entry *context_next;

				static GLfloat *buffer_coords(GLfloat *buffer, GLfloat x, GLfloat y, GLfloat x2, GLfloat y2);
				static GLshort *buffer_quad(GLshort *buffer, int x, int y, int width, int height);
			
				Entry(uint32_t entry_type) : entry_type(entry_type)
				{
				}
			
				virtual void measure(ContentMeasurer &measurer) = 0;
				virtual void serialize(ContentSerializer &serializer) = 0;
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
		LayerCanvas(MemoryPool &memory_pool);

		MemoryPool &memory_pool;

		template<class T, uint32_t entry_type> T *acquire_class()
		{
			T *result = (T *)lookup(entry_type);

			if(result)
				return result;

			result = new (memory_pool) T(memory_pool);

			store(result);

			return result;
		};
		
		Entry *lookup(uint32_t entry_type);
		void store(Entry *content);

		Layer *render();
	};
};
