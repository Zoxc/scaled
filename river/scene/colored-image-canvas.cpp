#include "../memory-pool.hpp"
#include "buffer.hpp"
#include "scene.hpp"
#include "../image.hpp"
#include "colored-image-canvas.hpp"
#include "content-serializer.hpp"

namespace River
{
	static const char* vertex_shader = "precision highp float;\
		uniform vec2 scene;\
		attribute vec2 point;\
		attribute vec2 tex_coord;\
		varying vec2 v_tex_coord;\
		attribute vec4 color;\
		varying vec4 v_color;\
		void main(void)\
		{\
			gl_Position = vec4(point.x / scene.x - 1.0, 1.0 - point.y / scene.y, 0.0, 1.0);\
			v_tex_coord = tex_coord;\
			v_color = color;\
		}";
	
	static const char* fragment_shader = "precision highp float;\
		varying vec2 v_tex_coord;\
		varying vec4 v_color;\
		uniform sampler2D texture;\
		void main(void)\
		{\
			gl_FragColor = v_color;\
			gl_FragColor.a *= texture2D(texture, v_tex_coord).a;\
		}";

	ColoredImageState::ColoredImageState() : ShaderState(vertex_shader, fragment_shader)
	{
	}

	void ColoredImageState::bind_attributes(GLuint program)
	{
		glBindAttribLocation(program, 0, "point");
		glBindAttribLocation(program, 1, "color");
		glBindAttribLocation(program, 2, "tex_coord");
	}

	void ColoredImageState::get_uniforms(GLuint program)
	{
		texture_uniform = glGetUniformLocation(program, "texture");
	}

	void ColoredImageState::use()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderState::use();

		glUniform1i(texture_uniform, 0);
	}
	
	void ColoredImageCanvas::Content::render(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this

		Scene::colored_image_state.use();
		
		for(ContentWalker::Iterator<ContentEntry> i = walker.read_list<ContentEntry>(); i.get_next();)
		{
			ContentEntry &entry = i();
			
			glBindTexture(GL_TEXTURE_2D, entry.texture);
			
			entry.vertex_buffer.bind();
			glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

			entry.color_buffer.bind();
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

			entry.coords_buffer.bind();
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, entry.indices);

			Scene::draw_call();
		}
	}

	void ColoredImageCanvas::Content::deallocate(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		for(ContentWalker::Iterator<ContentEntry> i = walker.read_list<ContentEntry>(); i.get_next();)
			i().~ContentEntry();
	}

	ColoredImageCanvas::Object::Object(int x, int y, int width, int height, color_t color, AtlasEntry *entry) : x(x), y(y), width(width), height(height), color(color), atlas_entry(entry)
	{
	}

	ColoredImageCanvas::ColoredImageCanvas(MemoryPool &memory_pool) : LayerCanvas::Entry(LayerCanvas::Entry::ColoredImageCanvas), object_table(memory_pool)
	{
	}

	ColoredImageCanvas *ColoredImageCanvas::acquire(LayerCanvas *layer)
	{
		return layer->acquire_class<ColoredImageCanvas, LayerCanvas::Entry::ColoredImageCanvas>();
	}
	
	void ColoredImageCanvas::render_image(LayerCanvas *layer, int x, int y, int width, int height, color_t color, Image *image)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height, color, &image->entry);
		
		object_table.table.get(image->texture)->append(object);
	}
	
	GLubyte *ColoredImageCanvas::buffer_color(GLubyte *buffer, uint32_t color)
	{
		*buffer++ = color_red_component(color);
		*buffer++ = color_green_component(color);
		*buffer++ = color_blue_component(color);
		*buffer++ = color_alpha_component(color);

		return buffer;
	}
	
	void ColoredImageCanvas::measure(ContentMeasurer &measurer)
	{
		measurer.count_objects<Content>(1);
		measurer.count_lists(1);
		measurer.count_objects<ContentEntry>(object_table.table.get_entries());
	}

	void ColoredImageCanvas::serialize(ContentSerializer &serializer)
	{
		serializer.write_object<Content>();
		serializer.write_list(object_table.table.get_entries());
		
		for(ObjectListHash::Table::Iterator i = object_table.table.begin(); i != object_table.table.end(); ++i)
		{
			ObjectList *list = *i;
			ContentEntry &entry = serializer.write_object<ContentEntry>();

			entry.texture = list->key->texture;
			entry.indices = list->size * 6;
			
			GLshort *vertex_map = entry.vertex_buffer.setup(entry.indices * 2);
			GLubyte *color_map = entry.color_buffer.setup(entry.indices * 4);
			GLfloat *coords_map = entry.coords_buffer.setup(entry.indices * 2);
			
			for(ObjectList::Iterator j = list->begin(); j != list->end(); ++j)
			{
				Object *object = *j;

				vertex_map = buffer_quad(vertex_map, object->x, object->y, object->width, object->height);
			
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);

				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);

				coords_map = buffer_coords(coords_map, object->atlas_entry->x, object->atlas_entry->y, object->atlas_entry->x2, object->atlas_entry->y2);
			}

			entry.vertex_buffer.unmap();
			entry.color_buffer.unmap();
			entry.coords_buffer.unmap();
		}
	}
};
