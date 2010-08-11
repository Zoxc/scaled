#pragma once
#include <stdint.h>
#include "../../hash-table.hpp"
#include "../../simple-list.hpp"
#include "../../ref-object.hpp"
#include "glyph.hpp"
#include "glyph-cache.hpp"

namespace River
{
	class FontSize;

	class FontGlyphFunctions:
		public HashTableFunctions<uint32_t, Glyph *, FontSize *>
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

		static Glyph *FontGlyphFunctions::create_value(FontSize *font_size, uint32_t key)
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
		FontGlyphTable table;
		FontSize *next;
		GlyphCacheList glyph_caches;
		GlyphCache *current_cache;

		GlyphCache *place(Glyph *glyph, size_t width, size_t height, void *raster);

		Glyph *get_glyph(uint32_t code);
	};
};
