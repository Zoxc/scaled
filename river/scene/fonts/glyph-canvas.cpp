#include "../../memory-pool.hpp"
#include "../buffer.hpp"
#include "../scene.hpp"
#include "../content-serializer.hpp"
#include "glyph-canvas.hpp"
#include "font-size.hpp"
#include "glyph.hpp"

namespace River
{
	void GlyphCanvas::Content::deallocate(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		for(ContentWalker::Iterator<GLuint> i = walker.read_list<GLuint>(); i.get_next();)
		{
			for(ContentWalker::Iterator<ColorKeyContent> j = walker.read_list<ColorKeyContent>(); j.get_next();)
				j().~ColorKeyContent();
		}
	}

	void GlyphCanvas::Content::render(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		Scene::glyph_state.use();
		
		for(ContentWalker::Iterator<GLuint> i = walker.read_list<GLuint>(); i.get_next();)
		{
			GLuint &texture = i();
			
			glBindTexture(GL_TEXTURE_2D, texture);
			
			for(ContentWalker::Iterator<ColorKeyContent> j = walker.read_list<ColorKeyContent>(); j.get_next();)
			{
				ColorKeyContent &key = j();

				glUniform1f(Scene::glyph_state.alpha_uniform, key.a);
				glBlendColor(key.r, key.g, key.b, 0.0);

				key.vertex_buffer.bind();
				glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

				key.coords_buffer.bind();
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

				glDrawArrays(GL_TRIANGLES, 0, key.indices);

				Scene::draw_call();
			}
		}
	}

	GlyphCanvas::Object::Object(int x, int y, Glyph *glyph, uint8_t offset) : x(x), y(y), glyph(glyph), offset(offset)
	{
	}

	GlyphCanvas::GlyphCanvas(MemoryPool &memory_pool) : LayerCanvas::Entry(LayerCanvas::Entry::GlyphCanvas), glyph_objects(memory_pool), glyph_lists(0)
	{
	}

	GlyphCanvas *GlyphCanvas::acquire(LayerCanvas *layer)
	{
		return layer->acquire_class<GlyphCanvas, LayerCanvas::Entry::GlyphCanvas>();
	}

	void GlyphCanvas::render_glyph(LayerCanvas *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color)
	{
		Object *object = new (layer->memory_pool) Object(x, y - glyph->offset_y, glyph, subpixel_offset);

		glyph_objects.table.get(glyph->offsets[subpixel_offset].cache)->table.get(color)->append(object);
	}
	
	void GlyphCanvas::render_text(LayerCanvas *layer, int x, int y, const char *text, FontSize *font_size, color_t color)
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
	
	void GlyphCanvas::measure(ContentMeasurer &measurer)
	{
		measurer.count_objects<Content>(1);
		
		measurer.count_lists(glyph_objects.table.get_entries() + 1);

		measurer.count_objects<GLuint>(glyph_objects.table.get_entries());
		
		// TODO: Calculate list count when adding objects
		for(GlyphObjectHash::Table::Iterator i = glyph_objects.table.begin(); i != glyph_objects.table.end(); ++i)
			measurer.count_objects<ColorKeyContent>((*i)->table.get_entries());
	}

	void GlyphCanvas::serialize(ContentSerializer &serializer)
	{
		serializer.write_object<Content>();
		serializer.write_list(glyph_objects.table.get_entries());
		
		for(GlyphObjectHash::Table::Iterator i = glyph_objects.table.begin(); i != glyph_objects.table.end(); ++i)
		{
			ColorKeyHash *hash = *i;

			GLuint &texture = serializer.write_object<GLuint>();
			texture = hash->key->texture;
			
			serializer.write_list(hash->table.get_entries());

			for(ColorKeyHash::Table::Iterator j = hash->table.begin(); j != hash->table.end(); ++j)
			{
				ColorKeyContent &key_content = serializer.write_object<ColorKeyContent>();
				GlyphObjectList *list = *j;
				
				key_content.r = color_red_component(list->key) / (GLclampf)255.0;
				key_content.g = color_green_component(list->key) / (GLclampf)255.0;
				key_content.b = color_blue_component(list->key) / (GLclampf)255.0;
				key_content.a = color_alpha_component(list->key) / (GLclampf)255.0;
				key_content.indices = list->size * 6;
				
				GLshort *vertex_map = key_content.vertex_buffer.setup(key_content.indices * 2);
				GLfloat *coords_map = key_content.coords_buffer.setup(key_content.indices * 2);
				
				for(GlyphObjectList::Iterator k = list->begin(); k != list->end(); ++k)
				{
					Object *object = *k;
					Glyph::Variation *variantion = &object->glyph->offsets[object->offset];
				
					vertex_map = buffer_quad(vertex_map, object->x, object->y, variantion->width, object->glyph->height);
					coords_map = buffer_coords(coords_map, variantion->entry.x, variantion->entry.y, variantion->entry.x2, variantion->entry.y2);
				}

				key_content.vertex_buffer.unmap();
				key_content.coords_buffer.unmap();
			}
		}
	}
};
