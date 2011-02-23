#pragma once
#include <vector>
#include "../color.hpp"
#include "../counted-simple-list.hpp"
#include "object-hash.hpp"
#include "layer-context.hpp"
#include "layer.hpp"

namespace River
{
	class Glyph;
	class GlyphCache;
	class Buffer;
	
	class GradientContext:
		public LayerContext::Entry
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
				Buffer *vertex_buffer;
				Buffer *color_buffer;

				void render();
		};

		typedef CountedSimpleList<Object> ObjectList;

		GLubyte *buffer_color(GLubyte *buffer, uint32_t color);

		ObjectList objects;
	public:
		GradientContext(MemoryPool &memory_pool) : LayerContext::Entry(LayerContext::Entry::GradientContext)
		{
		}

		static GradientContext *acquire(LayerContext *layer);
		
		void render_vertical(LayerContext *layer, int x, int y, int width, int height, color_t top, color_t bottom);
		void render_horizontal(LayerContext *layer, int x, int y, int width, int height, uint32_t left, uint32_t right);

		void render(Layer *layer);
	};
};
