#include "../memory-pool.hpp"
#include "buffer.hpp"
#include "scene.hpp"
#include "../image.hpp"
#include "colored-image-canvas.hpp"

namespace River
{
	static const char* vertex_shader = "precision highp float;\
		uniform vec2 scene;\
		attribute vec2 point;\
		attribute vec4 color;\
		varying vec4 v_color;\
		void main(void)\
		{\
			gl_Position = vec4(point.x / scene.x - 1.0, 1.0 - point.y / scene.y, 0.0, 1.0);\
			v_color = color;\
		}";
	
	static const char* fragment_shader = "precision highp float;\
		varying vec4 v_color;\
		void main(void)\
		{\
			gl_FragColor = v_color;\
		}";

	ColoredImageState::ColoredImageState() : ShaderState(vertex_shader, fragment_shader)
	{
	}

	void ColoredImageState::bind_attributes(GLuint program)
	{
		glBindAttribLocation(program, 0, "point");
		glBindAttribLocation(program, 1, "color");
	}

	void ColoredImageState::get_uniforms(GLuint program)
	{
	}

	void ColoredImageState::use()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderState::use();
	}
	
	ColoredImageCanvas::ContentEntry::~ContentEntry()
	{
		delete vertex_buffer;
		delete color_buffer;
	}

	ColoredImageCanvas::Content::~Content()
	{
		for(std::vector<ContentEntry *>::iterator i = list.begin(); i != list.end(); ++i)
			delete *i;
	}

	void ColoredImageCanvas::Content::render()
	{
		Scene::colored_image_state.use();
		
		for(std::vector<ContentEntry *>::iterator i = list.begin(); i != list.end(); ++i)
		{
			ContentEntry *entry = *i;
			
			glBindTexture(GL_TEXTURE_2D, entry->texture);
			
			entry->vertex_buffer->bind();
			glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

			entry->color_buffer->bind();
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, entry->indices);

			Scene::draw_call();
		}
	}

	ColoredImageCanvas::Object::Object(int x, int y, int width, int height, color_t color, Image *image) : x(x), y(y), width(width), height(height), color(color), image(image)
	{
	}

	ColoredImageCanvas *ColoredImageCanvas::acquire(LayerContext *layer)
	{
		return layer->acquire_class<ColoredImageCanvas, LayerContext::Entry::ColoredImageCanvas>();
	}
	
	void ColoredImageCanvas::render_image(LayerContext *layer, int x, int y, int width, int height, color_t color, Image *image)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height, color, image);
		
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

	void ColoredImageCanvas::render(Layer *layer)
	{
		Content *content = new Content;
		
		for(ObjectListHash::Table::Iterator i = object_table.table.begin(); i != object_table.table.end(); ++i)
		{
			ObjectList *list = *i;
			ContentEntry *entry = new ContentEntry;

			entry->indices = list->size * 6;
			entry->vertex_buffer = new Buffer(GL_ARRAY_BUFFER, entry->indices * 2 * sizeof(GLshort));
			entry->color_buffer = new Buffer(GL_ARRAY_BUFFER, entry->indices * 4 * sizeof(GLubyte));
		
			GLshort *vertex_map = (GLshort *)entry->vertex_buffer->map();
			GLubyte *color_map = (GLubyte *)entry->color_buffer->map();
			
			for(ObjectList::Iterator j = list->begin(); j != list->end(); ++i)
			{
				Object *object = *j;

				vertex_map = buffer_quad(vertex_map, object->x, object->y, object->width, object->height);
			
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);

				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);
				color_map = buffer_color(color_map, object->color);
			}

			entry->vertex_buffer->unmap();
			entry->color_buffer->unmap();

			content->list.push_back(entry);
		}

		layer->append(content);
	}
};
