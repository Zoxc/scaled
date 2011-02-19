#pragma once
#include <stdint.h>
#include "../../gles.hpp"
#include "../../freetype.hpp"

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
		
		#pragma pack(push, 1)
		struct TexCoord {
			GLfloat x;
			GLfloat y;
		};
		#pragma pack(pop)

		struct Variation {
			size_t width;
			GlyphCache *cache;
			TexCoord coords[4];
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
