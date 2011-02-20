#pragma once
#include <vector>
#include "../../color.hpp"
#include "../../counted-simple-list.hpp"
#include "../object-hash.hpp"
#include "../layer-context.hpp"

namespace River
{
	class Glyph;
	class GlyphCache;
	class Buffer;
	
	class GlyphContext:
		public LayerContext::Entry
	{
	private:
		class Object
		{
		public:
			Object(int x, int y, Glyph *glyph, uint8_t offset);

			SimpleEntry<Object> entry;
			
			int x;
			int y;
			Glyph *glyph;
			uint8_t offset;
		};
		
		class ContentList
		{
			public:
				GLuint texture;
				size_t indices;
				Buffer *vertex_buffer;
				Buffer *coords_buffer;
		};

		class Content:
			public Layer::Content
		{
			public:
				std::vector<ContentList *> list;

				void render();
		};

		class GlyphObjectList:
			public CountedSimpleList<Object>
		{
			public:
				GlyphCache *key;
				GlyphObjectList *next;
		};

		typedef ObjectHash<GlyphCache *, GlyphObjectList> GlyphObjectHash;

		GLshort *buffer_quad(GLshort *buffer, bool first, int x, int y, int width, int height);
		GLfloat *buffer_coords(GLfloat *buffer, bool first, Glyph::Variation *variant);

		GlyphObjectHash glyph_objects;
	public:
		GlyphContext(MemoryPool &memory_pool) : LayerContext::Entry(LayerContext::Entry::GlyphContext), glyph_objects(memory_pool)
		{
		}

		static GlyphContext *acquire(LayerContext *layer);

		void render_glyph(LayerContext *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color);

		void render(Layer *layer);
	};
};
