#include "glyph.hpp"
#include "font-size.hpp"
#include "font.hpp"

namespace River
{
	Glyph::Glyph(uint32_t code, FontSize *font_size) : code(code)
	{
		FT_Face face = font_size->font.face;

		if(font_size->font.current_size != font_size->size)
		{
			assert(FT_Set_Char_Size(face, 0, font_size->size * 64, 96, 96) == 0);
			font_size->font.current_size = font_size->size;
		}

		assert(FT_Load_Char(face, code, FT_LOAD_TARGET_LIGHT) == 0);
		assert(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LIGHT) == 0);
		
		width = face->glyph->bitmap.width;
		height = face->glyph->bitmap.rows;
		
		offset_x = face->glyph->bitmap_left;
		offset_y = face->glyph->bitmap_top;

		advance = face->glyph->advance.x >> 6;

		cache = font_size->place(this, width, height, face->glyph->bitmap.buffer);
	}
};
