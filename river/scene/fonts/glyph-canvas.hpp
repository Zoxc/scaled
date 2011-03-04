#pragma once
#include <vector>
#include "../../color.hpp"
#include "../../counted-simple-list.hpp"
#include "../object-hash.hpp"
#include "../layer-canvas.hpp"
#include "../atlas.hpp"
#include "../buffer.hpp"

namespace River
{
	class Glyph;
	class GlyphCache;
	
	class GlyphCanvas:
		public LayerCanvas::Entry
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
		
		class ColorKeyContent
		{
			public:
				GLclampf r, g, b, a;
				size_t indices;
				Buffer<GL_ARRAY_BUFFER, GLshort> vertex_buffer;
				Buffer<GL_ARRAY_BUFFER, GLfloat> coords_buffer;
		};

		class Content:
			public Layer::Content
		{
			public:
				void render(ContentWalker &walker);
				void deallocate(ContentWalker &walker);
		};

		class GlyphObjectList:
			public CountedSimpleList<Object>
		{
			public:
				GlyphObjectList(MemoryPool &memory_pool) {}
				
				color_t key;
				GlyphObjectList *next;
		};
		
		class ColorKeyHash:
			public ObjectHash<color_t, GlyphObjectList>
		{
			public:
				ColorKeyHash(MemoryPool &memory_pool) : ObjectHash<color_t, GlyphObjectList>(memory_pool) {}

				Atlas<GL_RGB>::Texture *key;
				ColorKeyHash *next;
		};

		typedef ObjectHash<Atlas<GL_RGB>::Texture *, ColorKeyHash> GlyphObjectHash;

		GlyphObjectHash glyph_objects;
		size_t glyph_lists;
	public:
		GlyphCanvas(MemoryPool &memory_pool);

		static GlyphCanvas *acquire(LayerCanvas *layer);
		
		void render_glyph(LayerCanvas *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color);
		void render_text(LayerCanvas *layer, int x, int y, const char *text, FontSize *font_size, color_t color);
		
		void measure(ContentMeasurer &measurer);
		void serialize(ContentSerializer &serializer);
	};
};
