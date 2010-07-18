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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, this->width, this->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	GlyphCache::~GlyphCache()
	{
		glDeleteTextures(1, &texture);
	}
	
	bool GlyphCache::place(Glyph *glyph, size_t width, size_t height, void *raster)
	{
		if(this->width - x < width)
		{
			x = 0;
			y += line_height;
			line_height = 0;
		}
		
		if(this->height - y < height)
			return false;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_ALPHA, GL_UNSIGNED_BYTE, raster);
		
		glyph->coords[0].x = (GLfloat)x / (GLfloat)this->width;
		glyph->coords[0].y = (GLfloat)y / (GLfloat)this->height;

		glyph->coords[1].x = ((GLfloat)x + width) / (GLfloat)this->width;
		glyph->coords[1].y = (GLfloat)y / (GLfloat)this->height;
		
		glyph->coords[2].x = (GLfloat)x / (GLfloat)this->width;
		glyph->coords[2].y = ((GLfloat)y + height) / (GLfloat)this->height;
		
		glyph->coords[3].x = ((GLfloat)x + width) / (GLfloat)this->width;
		glyph->coords[3].y = ((GLfloat)y + height) / (GLfloat)this->height;
		
		x += width;
		line_height = std::max(line_height, height);

		return true;
	}
};
