#include "../../memory-pool.hpp"
#include "../buffer.hpp"
#include "../scene.hpp"
#include "glyph-context.hpp"
#include "glyph.hpp"

namespace River
{
	void GlyphContext::Content::render()
	{
		Scene::glyph_state.use();

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

			glDrawArrays(GL_TRIANGLES, 0, list->indices);
			
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
		Object *object = new (layer->memory_pool) Object(x, y - glyph->offset_y, glyph, subpixel_offset);

		glyph_objects.table.get(glyph->offsets[subpixel_offset].cache)->append(object);
	}
	
	void GlyphContext::render_text(LayerContext *layer, int x, int y, const char *text, FontSize *font_size, color_t color)
	{
		int left = 0;

		while(*text)
		{
			Glyph *glyph = font_size->get_glyph(*text);

			int offset = left + glyph->offset_x;
			int subpixel_offset = offset % 3;

			// Make sure it's a positive number
			if(subpixel_offset < 0)
				subpixel_offset += 3;

			render_glyph(layer, x + offset / 3, y, glyph, subpixel_offset, color);

			left += glyph->advance;
			
			text++;
		}
	}
	
    GLshort *GlyphContext::buffer_quad(GLshort *buffer, bool first, int x, int y, int width, int height)
    {
		int right = x + width;
		int bottom = y + height;
		
        *buffer++ = x;
		*buffer++ = y;

        *buffer++ = right;
		*buffer++ = y;

        *buffer++ = x;
		*buffer++ = bottom;

        *buffer++ = right;
		*buffer++ = y;

        *buffer++ = x;
		*buffer++ = bottom;

        *buffer++ = right;
		*buffer++ = bottom;

        return buffer;
    }

    GLfloat *GlyphContext::buffer_coords(GLfloat *buffer, bool first, Glyph::Variation *variant)
    {
        *buffer++ = variant->coords[0].x;
		*buffer++ = variant->coords[0].y;
		
        *buffer++ = variant->coords[1].x;
		*buffer++ = variant->coords[1].y;

        *buffer++ = variant->coords[2].x;
		*buffer++ = variant->coords[2].y;

        *buffer++ = variant->coords[1].x;
		*buffer++ = variant->coords[1].y;
		
        *buffer++ = variant->coords[2].x;
		*buffer++ = variant->coords[2].y;

        *buffer++ = variant->coords[3].x;
		*buffer++ = variant->coords[3].y;

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
			content_list->indices = list->size * 6;
			content_list->vertex_buffer = new Buffer(GL_ARRAY_BUFFER, content_list->indices * 2 * sizeof(GLshort));
			content_list->coords_buffer = new Buffer(GL_ARRAY_BUFFER, content_list->indices * 2 * sizeof(GLfloat));
			
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

			content->list.push_back(content_list);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		layer->append(content);
	}
};
