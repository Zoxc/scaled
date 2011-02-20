#pragma once
#include "../../gles.hpp"
#include "../../simple-list.hpp"
#include "../object.hpp"
#include "../buffer.hpp"
#include "font-size.hpp"
#include "glyph-object.hpp"

namespace River
{
	class TextObject:
		public Object
	{
	private:
		FontSize *font_size;
		Layer *layer;
		color_t color;
		size_t indices;
		
		Buffer *vertex_buffer;
		Buffer *coords_buffer;
		
		GLshort *buffer_quad(GLshort *buffer, bool first, int x, int y, int width, int height);
		GLfloat *buffer_coords(GLfloat *buffer, bool first, Glyph::Variation *variant);
		
		void clear();
	public:
		TextObject();
		~TextObject();

		void render();
		void position(int x, int y, FontSize *font_size, color_t color, const char *text);
		void attach(Layer *layer);
	};
};
