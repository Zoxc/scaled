#pragma once
#include <vector>
#include "../color.hpp"
#include "../counted-simple-list.hpp"
#include "object-hash.hpp"
#include "layer-canvas.hpp"
#include "layer.hpp"
#include "buffer.hpp"

namespace River
{
	class Glyph;
	class GlyphCache;
	
	class GradientCanvas:
		public LayerCanvas::Entry
	{
	private:
		class Object
		{
		public:
			Object(int x, int y, int width, int height);

			SimpleEntry<Object> entry;
			
			int x;
			int y;
			int width;
			int height;
			color_t colors[4];
		};
		
		class Content:
			public Layer::Content
		{
			public:
				size_t indices;
				Buffer<GL_ARRAY_BUFFER, GLshort> vertex_buffer;
				Buffer<GL_ARRAY_BUFFER, GLubyte> color_buffer;

				void render(ContentWalker &walker);
				void deallocate(ContentWalker &walker);
		};

		typedef CountedSimpleList<Object> ObjectList;

		GLubyte *buffer_color(GLubyte *buffer, uint32_t color);

		ObjectList objects;
	public:
		GradientCanvas(MemoryPool &memory_pool);

		static GradientCanvas *acquire(LayerCanvas *layer);
		
		void render_vertical(LayerCanvas *layer, int x, int y, int width, int height, color_t top, color_t bottom);
		void render_horizontal(LayerCanvas *layer, int x, int y, int width, int height, color_t left, color_t right);
		
		void measure(ContentMeasurer &measurer);
		void serialize(ContentSerializer &serializer);
	};
};
