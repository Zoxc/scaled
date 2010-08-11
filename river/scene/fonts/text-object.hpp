#pragma once
#include "../../gles.hpp"
#include "../../simple-list.hpp"
#include "../object.hpp"
#include "font-size.hpp"
#include "glyph-object.hpp"

namespace River
{
	class TextObject
	{
	private:
		FontSize *font_size;

		void clear();
	public:
		TextObject();
		~TextObject();

		typedef SimpleList<GlyphObject, GlyphObject, &GlyphObject::text_entry> GlyphList;

		GlyphList glyph_list;

		void position(int x, int y, FontSize *font_size, color_t color, const char *text);
		
		void place(Layer *layer);
	};
};
