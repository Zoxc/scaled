#include "../memory-pool.hpp"
#include "buffer.hpp"
#include "scene.hpp"
#include "gradient-context.hpp"
#include "content-serializer.hpp"

namespace River
{
	void GradientContext::Content::render(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		Scene::gradient_state.use();

		vertex_buffer->bind();
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

		color_buffer->bind();
		glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, indices);

		Scene::draw_call();
	}

	void GradientContext::Content::deallocate(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		delete vertex_buffer;
		delete color_buffer;
	}

	GradientContext::Object::Object(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
	{
	}

	GradientContext::GradientContext(MemoryPool &memory_pool) : LayerContext::Entry(LayerContext::Entry::GradientContext)
	{
	}

	GradientContext *GradientContext::acquire(LayerContext *layer)
	{
		return layer->acquire_class<GradientContext, LayerContext::Entry::GradientContext>();
	}
	
	void GradientContext::render_vertical(LayerContext *layer, int x, int y, int width, int height, color_t top, color_t bottom)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height);
		
		object->colors[0] = top;
		object->colors[1] = top;
		object->colors[2] = bottom;
		object->colors[3] = bottom;

		objects.append(object);
	}

	void GradientContext::render_horizontal(LayerContext *layer, int x, int y, int width, int height, color_t left, color_t right)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height);
		
		object->colors[0] = left;
		object->colors[1] = right;
		object->colors[2] = left;
		object->colors[3] = right;

		objects.append(object);
	}
	
	GLubyte *GradientContext::buffer_color(GLubyte *buffer, uint32_t color)
	{
		*buffer++ = color_red_component(color);
		*buffer++ = color_green_component(color);
		*buffer++ = color_blue_component(color);

		return buffer;
	}
	
	void GradientContext::measure(ContentMeasurer &measurer)
	{
		measurer.count_objects<Content>(1);
	}

	void GradientContext::serialize(ContentSerializer &serializer)
	{
		Content &content = serializer.write_object<Content>();
		
		content.indices = objects.size * 6;
		content.vertex_buffer = new Buffer(GL_ARRAY_BUFFER, content.indices * 2 * sizeof(GLshort));
		content.color_buffer = new Buffer(GL_ARRAY_BUFFER, content.indices * 3 * sizeof(GLubyte));
		
		GLshort *vertex_map = (GLshort *)content.vertex_buffer->map();
		GLubyte *color_map = (GLubyte *)content.color_buffer->map();
			
		for(ObjectList::Iterator i = objects.begin(); i != objects.end(); ++i)
		{
			Object *object = *i;

			vertex_map = buffer_quad(vertex_map, object->x, object->y, object->width, object->height);
			
			color_map = buffer_color(color_map, object->colors[0]);
			color_map = buffer_color(color_map, object->colors[1]);
			color_map = buffer_color(color_map, object->colors[2]);

			color_map = buffer_color(color_map, object->colors[1]);
			color_map = buffer_color(color_map, object->colors[2]);
			color_map = buffer_color(color_map, object->colors[3]);
		}

		content.vertex_buffer->unmap();
		content.color_buffer->unmap();
	}
};
