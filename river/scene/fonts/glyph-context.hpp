#pragma once
#include <vector>
#include "../../color.hpp"
#include "../../counted-simple-list.hpp"
#include "../object-hash.hpp"
#include "../layer-context.hpp"
#include "../atlas.hpp"

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
		
		class ColorKeyContent
		{
			public:
				virtual ~ColorKeyContent();

				GLclampf r, g, b, a;
				size_t indices;
				Buffer *vertex_buffer;
				Buffer *coords_buffer;
		};

		class ContentList
		{
			public:
				virtual ~ContentList();
				
				GLuint texture;
				std::vector<ColorKeyContent *> keys;
		};

		class Content:
			public Layer::Content
		{
			public:
				virtual ~Content();

				std::vector<ContentList *> list;

				void render();
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
				ColorKeyHash(MemoryPool &memory_pool) : ObjectHash(memory_pool) {}

				Atlas<GL_RGB>::Texture *key;
				ColorKeyHash *next;
		};

		typedef ObjectHash<Atlas<GL_RGB>::Texture *, ColorKeyHash> GlyphObjectHash;

		GlyphObjectHash glyph_objects;
	public:
		GlyphContext(MemoryPool &memory_pool) : LayerContext::Entry(LayerContext::Entry::GlyphContext), glyph_objects(memory_pool)
		{
		}

		static GlyphContext *acquire(LayerContext *layer);
		
		void render_glyph(LayerContext *layer, int x, int y, Glyph *glyph, uint8_t subpixel_offset, color_t color);
		void render_text(LayerContext *layer, int x, int y, const char *text, FontSize *font_size, color_t color);

		void render(Layer *layer);
	};
};
