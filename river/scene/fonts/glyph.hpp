#pragma once
#include <stdint.h>
#include "../../gles.hpp"
#include "../../freetype.hpp"
#include "../atlas.hpp"

namespace River
{
	class GlyphCache;
	class FontSize;

	class Glyph
	{
	private:
		static uint8_t get_pixel(FT_GlyphSlot glyph, int x, int y);

		static uint8_t filter_5_pixel(FT_GlyphSlot glyph, int x, int y);
		void filter_5(FontSize *font_size, FT_GlyphSlot ft_glyph);
	public:
		Glyph(uint32_t code, FontSize *font_size);

		uint32_t code;
		
		struct Variation
		{
			size_t width;
			River::Atlas<GL_RGB>::Texture *cache; // TODO: Find out why this requires the River prefix!
			AtlasEntry entry;
		};

		Variation offsets[3];

		Variation *get_variation(size_t offset);

		size_t height;

		int offset_x;
		int offset_y;
		int advance;

		Glyph *next;
	};
};
