#include "glyph-object.hpp"
#include "glyph-cache.hpp"
#include "../layer.hpp"
#include "../scene.hpp"

namespace River
{
	GlyphObject::GlyphObject() : font_size(0), glyph(0)
	{
	}

	GlyphObject::~GlyphObject()
	{
		if(font_size)
			font_size->deref();
	}

	void GlyphObject::set_glyph(Glyph *glyph, uint32_t color)
	{
		this->color = color;
		this->glyph = glyph;
	}

	void GlyphObject::render_key(GlyphCache *cache)
	{
		glBindTexture(GL_TEXTURE_2D, cache->texture);
	}

	void GlyphObject::render()
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, glyph->coords);
		glUniform4f(Scene::glyph_state.color_uniform, (color & 0xFF) / (GLfloat)255.0, ((color >> 8) & 0xFF) / (GLfloat)255.0, ((color >> 16) & 0xFF) / (GLfloat)255.0, ((color >> 24) & 0xFF) / (GLfloat)255.0);

		Rectangle::render();
	}

	void GlyphObject::place(Layer *layer)
	{
		layer->glyph_object_hash.add(glyph->cache, this);
	}
};
