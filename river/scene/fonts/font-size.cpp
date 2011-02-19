#include "font-size.hpp"

namespace River
{
	FontSize::FontSize(Font &font, size_t size) : font(font), size(size), table(5, this), current_cache(0)
	{
	}

	GlyphCache *FontSize::place(Glyph::Variation *variation, size_t height, uint8_t *raster)
	{
		assert(variation->width <= GlyphCache::width && height <= GlyphCache::height);

		if(!current_cache)
			current_cache = new GlyphCache();

		while(!current_cache->place(variation, height, raster))
		{
			glyph_caches.append(current_cache);
			current_cache = new GlyphCache();
		}

		return current_cache;
	}

	Glyph *FontSize::get_glyph(uint32_t code)
	{
		return table.get(code);
	}
};
