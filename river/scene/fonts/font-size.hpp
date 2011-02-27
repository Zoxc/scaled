#pragma once
#include <stdint.h>
#include "../../hash-table.hpp"
#include "../../allocator.hpp"
#include "../../ref-object.hpp"
#include "../atlas.hpp"

namespace River
{
	class FontSize;
	class Glyph;

	class FontGlyphFunctions:
		public HashTableFunctions<uint32_t, Glyph *, FontSize *, StdLibAllocator>
	{
	public:
		static bool compare_key_value(uint32_t key, Glyph *value);
		static uint32_t get_key(Glyph *value);
		static Glyph *get_value_next(Glyph *value);
		static void set_value_next(Glyph *value, Glyph *next);
		static bool valid_key(uint32_t key);
		static bool create_value();
		static Glyph *create_value(StdLibAllocator::Ref alloc_ref, FontSize *font_size, uint32_t key);
	};

	typedef HashTable<uint32_t, Glyph *, FontSize *, FontGlyphFunctions> FontGlyphTable;

	class Font;

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
		Atlas<GL_RGB> glyph_atlas;

		Glyph *get_glyph(uint32_t code);
	};
};
