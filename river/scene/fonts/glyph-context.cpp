#include "../../memory-pool.hpp"
#include "../buffer.hpp"
#include "../scene.hpp"
#include "glyph-context.hpp"
#include "glyph.hpp"

namespace River
{
	GlyphContext::ColorKeyContent::~ColorKeyContent()
	{
		delete vertex_buffer;
		delete coords_buffer;
	}

	GlyphContext::ContentList::~ContentList()
	{
		for(std::vector<ColorKeyContent *>::iterator i = keys.begin(); i != keys.end(); ++i)
			delete *i;
	}

	GlyphContext::Content::~Content()
	{
		for(std::vector<ContentList *>::iterator i = list.begin(); i != list.end(); ++i)
			delete *i;
	}

	void GlyphContext::Content::render()
	{
		Scene::glyph_state.use();

		color_t color = color_white;

		for(std::vector<ContentList *>::iterator i = list.begin(); i != list.end(); ++i)
		{
			ContentList *list = *i;
			
			glBindTexture(GL_TEXTURE_2D, list->texture);
			
			for(std::vector<ColorKeyContent *>::iterator j = list->keys.begin(); j != list->keys.end(); ++j)
			{
				ColorKeyContent *key = *j;

				glUniform1f(Scene::glyph_state.alpha_uniform, key->a);
				glBlendColor(key->r, key->g, key->b, 0.0);

				key->vertex_buffer->bind();
				glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

				key->coords_buffer->bind();
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

				glDrawArrays(GL_TRIANGLES, 0, key->indices);

				Scene::draw_call();
			}
		}
	}

	GlyphContext::Object::Object(int x, int y, Glyph *glyph, uint8_t offset) : x(x), y(y), glyph(glyph), offset(offset)
	{
	}

	GlyphContext *GlyphContext::acquire(LayerContext *layer)
	{
		return layer->acquire_class<GlyphContext, LayerContext::Entry::GlyphContext>();
	}

	void GlyphContext::render_glyph(LayerContext *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color)
	{
		Object *object = new (layer->memory_pool) Object(x, y - glyph->offset_y, glyph, subpixel_offset);

		glyph_objects.table.get(glyph->offsets[subpixel_offset].cache)->table.get(color)->append(object);
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
	
	void GlyphContext::render(Layer *layer)
	{
		Content *content = new Content;

		for(GlyphObjectHash::Table::Iterator i = glyph_objects.table.begin(); i != glyph_objects.table.end(); ++i)
		{
			ColorKeyHash *hash = *i;

			ContentList *content_list = new ContentList;
			content_list->texture = hash->key->texture;

			for(ColorKeyHash::Table::Iterator j = hash->table.begin(); j != hash->table.end(); ++j)
			{
				ColorKeyContent *key_content = new ColorKeyContent;
				GlyphObjectList *list = *j;
				
				key_content->r = color_red_component(list->key) / (GLclampf)255.0;
				key_content->g = color_green_component(list->key) / (GLclampf)255.0;
				key_content->b = color_blue_component(list->key) / (GLclampf)255.0;
				key_content->a = color_alpha_component(list->key) / (GLclampf)255.0;
				key_content->indices = list->size * 6;
				key_content->vertex_buffer = new Buffer(GL_ARRAY_BUFFER, key_content->indices * 2 * sizeof(GLshort));
				key_content->coords_buffer = new Buffer(GL_ARRAY_BUFFER, key_content->indices * 2 * sizeof(GLfloat));
				
				GLshort *vertex_map = (GLshort *)key_content->vertex_buffer->map();
				GLfloat *coords_map = (GLfloat *)key_content->coords_buffer->map();
				
				for(GlyphObjectList::Iterator k = list->begin(); k != list->end(); ++k)
				{
					Object *object = *k;
					Glyph::Variation *variantion = &object->glyph->offsets[object->offset];
				
					vertex_map = buffer_quad(vertex_map, object->x, object->y, variantion->width, object->glyph->height);
					coords_map = buffer_coords(coords_map, variantion->x, variantion->y, variantion->x2, variantion->y2);
				}

				key_content->vertex_buffer->unmap();
				key_content->coords_buffer->unmap();
				
				content_list->keys.push_back(key_content);
			}

			content->list.push_back(content_list);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		layer->append(content);
	}
};
