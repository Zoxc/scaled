#include "../../memory-pool.hpp"
#include "../buffer.hpp"
#include "../scene.hpp"
#include "glyph-context.hpp"
#include "glyph.hpp"

namespace River
{
	void GlyphContext::Content::render()
	{
		color_t color = color_black;

		for(std::vector<ContentList *>::iterator i = list.begin(); i != list.end(); ++i)
		{
			ContentList *list = *i;
			
			glBindTexture(GL_TEXTURE_2D, list->texture);

			glUniform1f(Scene::glyph_state.alpha_uniform, color_alpha_component(color) / (GLfloat)255.0);
			glBlendColor(color_red_component(color) / (GLfloat)255.0, color_green_component(color) / (GLfloat)255.0, color_blue_component(color) / (GLfloat)255.0, 0.0);

			list->vertex_buffer->bind();
			glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

			list->coords_buffer->bind();
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, list->indices);
			
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	GlyphContext::Object::Object(int x, int y, Glyph *glyph, uint8_t offset) : x(x), y(y), glyph(glyph), offset(offset)
	{
	}

	GlyphContext *GlyphContext::acquire(LayerContext *layer)
	{
		GlyphContext *result = (GlyphContext *)layer->lookup(LayerContext::Entry::GlyphContext);

		if(result)
			return result;

		result = new (layer->memory_pool) GlyphContext(layer->memory_pool);

		layer->store(result);

		return result;
	}

	void GlyphContext::render_glyph(LayerContext *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color)
	{
		Object *object = new (layer->memory_pool) Object(x, y, glyph, subpixel_offset);

		glyph_objects.table.get(glyph->offsets[subpixel_offset].cache)->append(object);
	}
	
    GLshort *GlyphContext::buffer_quad(GLshort *buffer, bool first, int x, int y, int width, int height)
    {
		int right = x + width;
		int bottom = y + height;
		
        if(!first)
        {
            *buffer++ = x;
			*buffer++ = y;
        }

        *buffer++ = x;
		*buffer++ = y;

        *buffer++ = right;
		*buffer++ = y;

        *buffer++ = x;
		*buffer++ = bottom;

        *buffer++ = right;
		*buffer++ = bottom;

        if(first)
        {
            *buffer++ = right;
			*buffer++ = bottom;
        }

        return buffer;
    }

    GLfloat *GlyphContext::buffer_coords(GLfloat *buffer, bool first, Glyph::Variation *variant)
    {
        if(!first)
        {
            *buffer++ = variant->coords[0].x;
			*buffer++ = variant->coords[0].y;
        }

        for(int i = 0; i < 4; i++)
		{
            *buffer++ = variant->coords[i].x;
			*buffer++ = variant->coords[i].y;
		}

        if(first)
        {
            *buffer++ = variant->coords[3].x;
			*buffer++ = variant->coords[3].y;
        }

        return buffer;
    }

	void GlyphContext::render(Layer *layer)
	{
		Content *content = new Content;

		for(GlyphObjectHash::Table::Iterator i = glyph_objects.table.begin(); i != glyph_objects.table.end(); ++i)
		{
			GlyphObjectList *list = *i;

			ContentList *content_list = new ContentList;

			content_list->texture = list->key->texture;
			content_list->indices = list->size * 10;
			content_list->vertex_buffer = new Buffer(GL_ARRAY_BUFFER, content_list->indices * sizeof(GLshort));
			content_list->coords_buffer = new Buffer(GL_ARRAY_BUFFER, content_list->indices * sizeof(GLfloat));
			
			GLshort *vertex_map = (GLshort *)content_list->vertex_buffer->map();
			GLfloat *coords_map = (GLfloat *)content_list->coords_buffer->map();
			
			 bool first = true;

			for(GlyphObjectList::Iterator j = list->begin(); j != list->end(); ++j)
			{
				Object *object = *j;
				Glyph::Variation *variantion = &object->glyph->offsets[object->offset];
				
				vertex_map = buffer_quad(vertex_map, first, object->x, object->y, variantion->width, object->glyph->height);
				coords_map = buffer_coords(coords_map, first, variantion);

				first = false;
			}

			content_list->vertex_buffer->unmap();
			content_list->coords_buffer->unmap();
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};
