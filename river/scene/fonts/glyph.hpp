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
