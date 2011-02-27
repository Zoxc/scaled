#include <algorithm>
#include "font-size.hpp"
#include "font.hpp"
#include "glyph.hpp"

namespace River
{
	bool FontGlyphFunctions::compare_key_value(uint32_t key, Glyph *value)
	{
		return key == value->code;
	}

	uint32_t FontGlyphFunctions::get_key(Glyph *value)
	{
		return value->code;
	}

	Glyph *FontGlyphFunctions::get_value_next(Glyph *value)
	{
		return value->next;
	}

	void FontGlyphFunctions::set_value_next(Glyph *value, Glyph *next)
	{
		value->next = next;
	}

	bool FontGlyphFunctions::valid_key(uint32_t key)
	{
		return true;
	}

	bool FontGlyphFunctions::create_value()
	{
		return true;
	}

	Glyph *FontGlyphFunctions::create_value(StdLibAllocator::Ref alloc_ref, FontSize *font_size, uint32_t key)
	{
		return new Glyph(key, font_size);
	}

	FontSize::FontSize(Font &font, size_t size) : font(font), size(size), table(5, this)
	{
		FT_Face face = font.use_size(size);

		assert(FT_Load_Char(face, 'W', FT_LOAD_FORCE_AUTOHINT) == 0);
		
		FT_Glyph_Metrics *metrics = &face->glyph->metrics;
		
		size_t up = metrics->horiBearingY;
		size_t down = metrics->horiBearingY - metrics->height;
		
		assert(FT_Load_Char(face, 'g', FT_LOAD_FORCE_AUTOHINT) == 0);
		
		metrics = &face->glyph->metrics;
		
		up = std::max<FT_Pos>(up, metrics->horiBearingY);
		down = std::max<FT_Pos>(down, metrics->horiBearingY - metrics->height);

		line_height = (up + down) >> 6;		
	}

	Glyph *FontSize::get_glyph(uint32_t code)
	{
		return table.get(code);
	}
};
