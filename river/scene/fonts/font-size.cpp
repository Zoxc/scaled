#include <algorithm>
#include "font-size.hpp"
#include "font.hpp"

namespace River
{
	FontSize::FontSize(Font &font, size_t size) : font(font), size(size), table(5, this), current_cache(0)
	{
		FT_Face face = font.use_size(size);

		assert(FT_Load_Char(face, 'W', FT_LOAD_FORCE_AUTOHINT) == 0);
		
		FT_Glyph_Metrics *metrics = &face->glyph->metrics;
		
		size_t up = metrics->horiBearingY;
		size_t down = metrics->horiBearingY - metrics->height;
		
		assert(FT_Load_Char(face, 'g', FT_LOAD_FORCE_AUTOHINT) == 0);
		
		metrics = &face->glyph->metrics;
		
		up = std::max<FT_Pos>(up, metrics->horiBearingY);
		down = std::max<FT_Pos>(down, metrics->horiBearingY - metrics->height);

		line_height = (up + down) >> 6;		
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
