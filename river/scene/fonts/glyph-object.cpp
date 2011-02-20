#include "glyph-object.hpp"
#include "glyph-cache.hpp"
#include "../layer.hpp"
#include "../scene.hpp"

namespace River
{
	GlyphObject::GlyphObject() : glyph(0)
	{
	}

	GlyphObject::~GlyphObject()
	{
	}

	void GlyphObject::set_glyph(Glyph *glyph, uint8_t subpixel_offset, color_t color)
	{
		this->color = color;
		this->offset = subpixel_offset;
		this->glyph = glyph;
	}

	void GlyphObject::render_key(GlyphCache *cache)
	{
		glBindTexture(GL_TEXTURE_2D, cache->texture);
	}

	void GlyphObject::render()
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, glyph->offsets[offset].coords);
		glUniform1f(Scene::glyph_state.alpha_uniform, color_alpha_component(color) / (GLfloat)255.0);
		glBlendColor(color_red_component(color) / (GLfloat)255.0, color_green_component(color) / (GLfloat)255.0, color_blue_component(color) / (GLfloat)255.0, 0.0);

		Rectangle::render();
	}

	void GlyphObject::attach(Layer *layer)
	{
		assert(offset < 3);

		//layer->glyph_object_hash.add(glyph->offsets[offset].cache, this);
	}

	void GlyphObject::deattach(Layer *layer)
	{
		//layer->glyph_object_hash.remove(glyph->offsets[offset].cache, this);
	}
};
