#pragma once
#include "../../gles.hpp"
#include "../../simple-list.hpp"
#include "../../ref-object.hpp"

namespace River
{
	class Glyph;
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
		static const GLfloat point_size;

		GLuint texture;
		SimpleEntry<GlyphCache> entry;

		bool place(Glyph *glyph, size_t width, size_t height, void *raster);
	};

};
