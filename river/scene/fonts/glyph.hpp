#pragma once
#include <stdint.h>
#include "../../freetype.hpp"
#include "glyph-cache.hpp"

namespace River
{
	class GlyphCache;
	class FontSize;

	class Glyph
	{
	private:
		typedef void (Glyph::*filter_t)(FontSize *font_size, FT_GlyphSlot glyph);
		
		static uint8_t get_pixel(FT_GlyphSlot glyph, int x, int y);

		static uint8_t filter_5_pixel(FT_GlyphSlot glyph, int x, int y);
		void filter_5(FontSize *font_size, FT_GlyphSlot glyph);

		void filter_dummy(FontSize *font_size, FT_GlyphSlot glyph);

		void place(FontSize *font_size, size_t width, size_t height, void *raster);

		static filter_t filter;
	public:
		Glyph(uint32_t code, FontSize *font_size);

		uint32_t code;
		GlyphCache *cache;
		
		#pragma pack(push, 1)
		struct TexCoord {
			GLfloat x;
			GLfloat y;
		};
		#pragma pack(pop)

		TexCoord coords[4];

		size_t width;
		size_t height;

		int offset_x;
		int offset_y;
		int advance;

		Glyph *next;
	};
};
