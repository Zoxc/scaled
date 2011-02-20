#include <algorithm>
#include "glyph-cache.hpp"
#include "glyph.hpp"
#include "font-size.hpp"

namespace River
{
	GlyphCache::GlyphCache() : x(0), y(0), line_height(0)
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GlyphCache::~GlyphCache()
	{
		glDeleteTextures(1, &texture);
	}
	
	bool GlyphCache::place(Glyph::Variation *variation, size_t height, uint8_t *raster)
	{
		size_t width = variation->width;

		if(this->width - x < width)
		{
			x = 0;
			y += line_height;
			line_height = 0;
		}
		
		if(this->height - y < height)
			return false;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, raster);
		
		const int offset = 0;

		variation->coords[0].x = (GLfloat)(x + offset) / (GLfloat)this->width;
		variation->coords[0].y = (GLfloat)y / (GLfloat)this->height;

		variation->coords[1].x = (GLfloat)(x + width) / (GLfloat)this->width;
		variation->coords[1].y = (GLfloat)y / (GLfloat)this->height;
		
		variation->coords[2].x = (GLfloat)(x + offset) / (GLfloat)this->width;
		variation->coords[2].y = (GLfloat)(y + height) / (GLfloat)this->height;
		
		variation->coords[3].x = (GLfloat)(x + width) / (GLfloat)this->width;
		variation->coords[3].y = (GLfloat)(y + height) / (GLfloat)this->height;
		
		x += width;
		line_height = std::max(line_height, height);

		return true;
	}
};
