#pragma once
#include "../../gles.hpp"
#include "../rectangle.hpp"
#include "font-size.hpp"

namespace River
{
	class GlyphCache;

	class GlyphObject:
		public Rectangle
	{
	private:
		FontSize *font_size;
		Glyph *glyph;
		uint32_t color;
	public:
		GlyphObject();
		~GlyphObject();

		void set_glyph(Glyph *glyph, uint32_t color);
		
		void render();
		static void render_key(GlyphCache *cache);
		void place(Layer *layer);
	};
};
