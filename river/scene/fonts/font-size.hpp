#pragma once
#include <stdint.h>
#include "../../hash-table.hpp"
#include "../../allocator.hpp"
#include "../../simple-list.hpp"
#include "../../ref-object.hpp"
#include "glyph.hpp"
#include "glyph-cache.hpp"

namespace River
{
	class FontSize;

	class FontGlyphFunctions:
		public HashTableFunctions<uint32_t, Glyph *, FontSize *, StdLibAllocator>
	{
	public:
		static bool compare_key_value(uint32_t key, Glyph *value)
		{
			return key == value->code;
		}

		static uint32_t get_key(Glyph *value)
		{
			return value->code;
		}

		static Glyph *get_value_next(Glyph *value)
		{
			return value->next;
		}

		static void set_value_next(Glyph *value, Glyph *next)
		{
			value->next = next;
		}

		static bool valid_key(uint32_t key)
		{
			return true;
		}

		static bool create_value()
		{
			return true;
		}

		static Glyph *create_value(StdLibAllocator::Ref alloc_ref, FontSize *font_size, uint32_t key)
		{
			return new Glyph(key, font_size);
		}
	};

	typedef HashTable<uint32_t, Glyph *, FontSize *, FontGlyphFunctions> FontGlyphTable;

	class Font;

	typedef SimpleList<GlyphCache, GlyphCache, &GlyphCache::entry> GlyphCacheList;

	class FontSize:
		public RefObject
	{
	public:
		FontSize(Font &font, size_t size);
		
		Font &font;
		size_t size;
		size_t line_height;
		FontGlyphTable table;
		FontSize *next;
		GlyphCacheList glyph_caches;
		GlyphCache *current_cache;

		GlyphCache *place(Glyph::Variation *variation, size_t height, uint8_t *raster);

		Glyph *get_glyph(uint32_t code);
	};
};
