#pragma once
#include <vector>
#include "../color.hpp"
#include "../counted-simple-list.hpp"
#include "object-hash.hpp"
#include "layer-context.hpp"
#include "layer.hpp"
#include "shader-state.hpp"
#include "atlas.hpp"
#include "buffer.hpp"

namespace River
{
	class Image;
	
	class ColoredImageState:
		public ShaderState
	{
	public:
		ColoredImageState();

		GLuint texture_uniform;

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
			Object(int x, int y, int width, int height, color_t color, AtlasEntry *entry);

			SimpleEntry<Object> entry;
			
			int x;
			int y;
			int width;
			int height;
			color_t color;
			AtlasEntry *atlas_entry;
		};
		
		class ContentEntry
		{
			public:
				
				GLuint texture;
				size_t indices;
				Buffer<GL_ARRAY_BUFFER, GLshort> vertex_buffer;
				Buffer<GL_ARRAY_BUFFER, GLubyte> color_buffer;
				Buffer<GL_ARRAY_BUFFER, GLfloat> coords_buffer;
		};

		class Content:
			public Layer::Content
		{
			public:
				void render(ContentWalker &walker);
				void deallocate(ContentWalker &walker);
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
		ColoredImageCanvas(MemoryPool &memory_pool);

		static ColoredImageCanvas *acquire(LayerContext *layer);
		
		void render_image(LayerContext *layer, int x, int y, int width, int height, color_t color, Image *image);

		void measure(ContentMeasurer &measurer);
		void serialize(ContentSerializer &serializer);
	};
};
