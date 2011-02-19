#include <math.h>
#include "glyph.hpp"
#include "font-size.hpp"
#include "font.hpp"

namespace River
{
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
			get_pixel(glyph, x - 2, y) * 1 +
			get_pixel(glyph, x - 1, y) * 3 +
			get_pixel(glyph, x + 0, y) * 8 +
			get_pixel(glyph, x + 1, y) * 3 +
			get_pixel(glyph, x + 2, y) * 1;
	
		return result / 16;
	}

	static int div_round_up(int dividend, int divisor)
	{
		return dividend % divisor ? (dividend / divisor) + 1 : dividend / divisor;
	}

	void Glyph::filter_5(FontSize *font_size, FT_GlyphSlot glyph)
	{
		const int border = 2;

		height = glyph->bitmap.rows;
		
		size_t width = glyph->bitmap.width + border * 2;
		size_t raster_width = div_round_up(width, 3);

		offsets[0].width = raster_width;
		
		offset_x = glyph->bitmap_left - border;
		offset_y = glyph->bitmap_top;

		uint8_t *raster = (uint8_t *)malloc(raster_width * 3 * height);
		uint8_t *pixel = raster;
		
		for(size_t y = 0; y < height; y++)
		{
			for(size_t x = 0; x < (raster_width * 3); x++)
			{
				*pixel++ = filter_5_pixel(glyph, x - border, y);
			}
		}
		
		advance = glyph->advance.x >> 6;
		
		offsets[0].cache = font_size->place(&offsets[0], height, raster);

		for(size_t i = 1; i < 3; ++i)
		{
			size_t variant_width = div_round_up(width + i, 3);
			offsets[i].width = variant_width;

			uint8_t *variant_raster = (uint8_t *)malloc(variant_width * 3 * height);

			uint8_t *variant_pixel = variant_raster;
			
			for(size_t y = 0; y < height; y++)
			{
				for(size_t x = 0; x < i; x++)
					*variant_pixel++ = 0;

				for(size_t x = 0; x < (variant_width * 3 - i); x++)
				{
					if(x < width)
						*variant_pixel++ = raster[(y * raster_width * 3) + x];
					else
						*variant_pixel++ = 0;
				}
			}

			offsets[i].cache = font_size->place(&offsets[i], height, variant_raster);

			free(variant_raster);
		}

		free(raster);
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

		filter_5(font_size, face->glyph);
	}
};
