#include "layer-canvas.hpp"
#include "layer-context.hpp"
#include "content-serializer.hpp"

namespace River
{

	GLshort *LayerCanvas::Entry::buffer_quad(GLshort *buffer, int x, int y, int width, int height)
	{
		int right = x + width;
		int bottom = y + height;

		*buffer++ = x;
		*buffer++ = y;

		*buffer++ = right;
		*buffer++ = y;

		*buffer++ = x;
		*buffer++ = bottom;

		*buffer++ = right;
		*buffer++ = y;

		*buffer++ = x;
		*buffer++ = bottom;

		*buffer++ = right;
		*buffer++ = bottom;

		return buffer;
	}

	GLfloat *LayerCanvas::Entry::buffer_coords(GLfloat *buffer, GLfloat x, GLfloat y, GLfloat x2, GLfloat y2)
	{
		*buffer++ = x;
		*buffer++ = y;

		*buffer++ = x2;
		*buffer++ = y;

		*buffer++ = x;
		*buffer++ = y2;

		*buffer++ = x2;
		*buffer++ = y;

		*buffer++ = x;
		*buffer++ = y2;

		*buffer++ = x2;
		*buffer++ = y2;

		return buffer;
	}

	LayerCanvas::LayerCanvas(LayerContext *context, MemoryPool &memory_pool) : map(2, false, memory_pool), context(context), memory_pool(memory_pool)
	{
	}
	
	void LayerCanvas::measure(ContentMeasurer &measurer)
	{
		measurer.count_objects<size_t>(map.get_entries());

		for(EntryMap::Iterator i = map.begin(); i != map.end(); ++i)
			(*i)->measure(measurer);
	}

	void LayerCanvas::serialize(ContentSerializer &serializer)
	{
		for(EntryMap::Iterator i = map.begin(); i != map.end(); ++i)
		{
			(*i)->serialize(serializer);

			// TODO: Add a conditional for debug test
			size_t &magic = serializer.write_object<size_t>();
			magic = 0xBEEF;
		}
	}

	LayerCanvas::Entry *LayerCanvas::lookup(uint32_t entry_type)
	{
		return map.get(entry_type);
	}

	void LayerCanvas::store(Entry *content)
	{
		map.set(content->entry_type, content);
	}

	LayerCanvas *LayerCanvas::raise()
	{
		if(entry.next)
			return entry.next;
		else
			return context->add_layer();
	}
};
