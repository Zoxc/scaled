#pragma once
#include <stdint.h>
#include "../../gles.hpp"
#include "../../simple-list.hpp"
#include "../../ref-object.hpp"
#include "glyph.hpp"

namespace River
{
	class FontSize;

	class GlyphCache
	{
	private:
		size_t x;
		size_t y;
		size_t line_height;

	public:
		GlyphCache();
		~GlyphCache();

		static const size_t width = 256;
		static const size_t height = 128;

		GLuint texture;
		SimpleEntry<GlyphCache> entry;

		bool place(Glyph::Variation *variation, size_t height, uint8_t *raster);
	};

};
