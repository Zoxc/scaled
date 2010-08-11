#include <math.h>
#include "glyph.hpp"
#include "font-size.hpp"
#include "font.hpp"

namespace River
{
	Glyph::filter_t Glyph::filter = &Glyph::filter_5;
	
	uint8_t Glyph::get_pixel(FT_GlyphSlot glyph, int x, int y)
	{
		if(x < 0 || x >= glyph->bitmap.width)
			return 0;
		
		if(glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
		{
			unsigned char *row = (unsigned char *)glyph->bitmap.buffer + glyph->bitmap.pitch * y;
			int byte = x / 8;
			int bit = 7 - (x % 8);
			return ((row[byte] >> bit) & 1) * 255;
		}
		else
			return ((uint8_t *)glyph->bitmap.buffer)[glyph->bitmap.pitch * y + x];
	}

	uint8_t Glyph::filter_5_pixel(FT_GlyphSlot glyph, int x, int y)
	{
		size_t result =
			get_pixel(glyph, x - 2, y) * 0 +
			get_pixel(glyph, x - 1, y) * 0 +
			get_pixel(glyph, x + 0, y) * 1 +
			get_pixel(glyph, x + 1, y) * 0 +
			get_pixel(glyph, x + 2, y) * 0;
	
		return result / 1;
	}

	static int div_round_up(int dividend, int divisor)
	{
		return dividend % divisor ? (dividend / divisor) + 1 : dividend / divisor;
	}

	void Glyph::filter_5(FontSize *font_size, FT_GlyphSlot glyph)
	{
		const int border = 1;
		const int offset = 2;

		/*
		 * Add 2 pixel borders on each size and align the size to pixels.
		 */
		size_t width = div_round_up(glyph->bitmap.width + border * 2, 3);

		this->width = width;
		this->height = glyph->bitmap.rows;
		
		/*
		 * Convert previous width to subpixels and add 2 subpixels used to offset the glyph.
		 */
		width = width * 3 + offset;
		
		offset_x = glyph->bitmap_left - border - offset;
		offset_y = glyph->bitmap_top;

		uint8_t *raster = (uint8_t *)malloc(width * height);
		uint8_t *pixel = raster;
		
		for(size_t y = 0; y < height; y++)
		{
			for(size_t x = 0; x < width; x++)
			{
				*pixel++ = filter_5_pixel(glyph, x - border - offset, y);
			}
		}
		
		advance = glyph->advance.x >> 6;

		place(font_size, width, height, raster);

		free(raster);
	}

	void Glyph::filter_dummy(FontSize *font_size, FT_GlyphSlot glyph)
	{
		width = glyph->bitmap.width;
		height = glyph->bitmap.rows;
		
		offset_x = glyph->bitmap_left;
		offset_y = glyph->bitmap_top;

		advance = glyph->advance.x >> 6;

		place(font_size, width, height, glyph->bitmap.buffer);
	}

	Glyph::Glyph(uint32_t code, FontSize *font_size) : code(code)
	{
		FT_Face face = font_size->font.face;

		if(font_size->font.current_size != font_size->size)
		{
			assert(FT_Set_Char_Size(face, 0, font_size->size * 64, 3 * 96, 96) == 0);
			font_size->font.current_size = font_size->size;
		}

		assert(FT_Load_Char(face, code, FT_LOAD_FORCE_AUTOHINT) == 0);
		assert(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) == 0);

		(this->*filter)(font_size, face->glyph);
	}

	void Glyph::place(FontSize *font_size, size_t width, size_t height, void *raster)
	{
		cache = font_size->place(this, width, height, raster);
	}
};
