#pragma once
#include <vector>
#include "../color.hpp"
#include "../counted-simple-list.hpp"
#include "object-hash.hpp"
#include "layer-context.hpp"
#include "layer.hpp"
#include "shader-state.hpp"
#include "atlas.hpp"

namespace River
{
	class Buffer;
	class Image;
	
	class ColoredImageState:
		public ShaderState
	{
	public:
		ColoredImageState();

		void bind_attributes(GLuint program);
		void get_uniforms(GLuint program);
		void use();
	};

	class ColoredImageCanvas:
		public LayerContext::Entry
	{
	private:
		class Object
		{
		public:
			Object(int x, int y, int width, int height, color_t color, Image *image);

			SimpleEntry<Object> entry;
			
			int x;
			int y;
			int width;
			int height;
			color_t color;
			Image *image;
		};
		
		class ContentEntry
		{
			public:
				~ContentEntry();
				
				GLuint texture;
				size_t indices;
				Buffer *vertex_buffer;
				Buffer *color_buffer;

				void render();
		};

		class Content:
			public Layer::Content
		{
			public:
				virtual ~Content();

				std::vector<ContentEntry *> list;

				void render();
		};
		
		class ObjectList:
			public CountedSimpleList<Object>
		{
			public:
				ObjectList(MemoryPool &memory_pool) {}
				
				Atlas<GL_RGBA>::Texture *key;
				ObjectList *next;
		};
		
		GLubyte *buffer_color(GLubyte *buffer, uint32_t color);

		typedef ObjectHash<Atlas<GL_RGBA>::Texture *, ObjectList> ObjectListHash;

		ObjectListHash object_table;
	public:
		ColoredImageCanvas(MemoryPool &memory_pool) : LayerContext::Entry(LayerContext::Entry::ColoredImageCanvas), object_table(memory_pool)
		{
		}

		static ColoredImageCanvas *acquire(LayerContext *layer);
		
		void render_image(LayerContext *layer, int x, int y, int width, int height, color_t color, Image *image);

		void render(Layer *layer);
	};
};
