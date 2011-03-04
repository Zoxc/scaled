#include "../memory-pool.hpp"
#include "buffer.hpp"
#include "scene.hpp"
#include "gradient-canvas.hpp"
#include "content-serializer.hpp"

namespace River
{
	void GradientCanvas::Content::render(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		Scene::gradient_state.use();

		vertex_buffer.bind();
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

		color_buffer.bind();
		glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, indices);

		Scene::draw_call();
	}

	void GradientCanvas::Content::deallocate(ContentWalker &walker)
	{
		walker.read_object<Content>(); // Skip this
		
		this->~Content();
	}

	GradientCanvas::Object::Object(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
	{
	}

	GradientCanvas::GradientCanvas(MemoryPool &memory_pool) : LayerCanvas::Entry(LayerCanvas::Entry::GradientCanvas)
	{
	}

	GradientCanvas *GradientCanvas::acquire(LayerCanvas *layer)
	{
		return layer->acquire_class<GradientCanvas, LayerCanvas::Entry::GradientCanvas>();
	}
	
	void GradientCanvas::render_vertical(LayerCanvas *layer, int x, int y, int width, int height, color_t top, color_t bottom)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height);
		
		object->colors[0] = top;
		object->colors[1] = top;
		object->colors[2] = bottom;
		object->colors[3] = bottom;

		objects.append(object);
	}

	void GradientCanvas::render_horizontal(LayerCanvas *layer, int x, int y, int width, int height, color_t left, color_t right)
	{
		Object *object = new (layer->memory_pool) Object(x, y, width, height);
		
		object->colors[0] = left;
		object->colors[1] = right;
		object->colors[2] = left;
		object->colors[3] = right;

		objects.append(object);
	}
	
	GLubyte *GradientCanvas::buffer_color(GLubyte *buffer, uint32_t color)
	{
		*buffer++ = color_red_component(color);
		*buffer++ = color_green_component(color);
		*buffer++ = color_blue_component(color);

		return buffer;
	}
	
	void GradientCanvas::measure(ContentMeasurer &measurer)
	{
		measurer.count_objects<Content>(1);
	}

	void GradientCanvas::serialize(ContentSerializer &serializer)
	{
		Content &content = serializer.write_object<Content>();
		
		content.indices = objects.size * 6;
		
		GLshort *vertex_map = content.vertex_buffer.setup(content.indices * 2);
		GLubyte *color_map = content.color_buffer.setup(content.indices * 3);
			
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

		content.vertex_buffer.unmap();
		content.color_buffer.unmap();
	}
};
