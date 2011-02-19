#pragma once
#include "../../color.hpp"
#include "../../gles.hpp"
#include "../../simple-list.hpp"
#include "../rectangle.hpp"

namespace River
{
	class Glyph;
	class GlyphCache;

	class GlyphObject:
		public Rectangle
	{
	private:
		Glyph *glyph;
		color_t color;
		uint8_t offset;
	public:
		GlyphObject();
		~GlyphObject();

		SimpleEntry<GlyphObject> text_entry;

		void set_glyph(Glyph *glyph, uint8_t subpixel_offset, color_t color);
		
		void render();
		static void render_key(GlyphCache *cache);
		void place(Layer *layer);
	};
};
