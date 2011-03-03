#include "layer-context.hpp"
#include "layer.hpp"
#include "content-serializer.hpp"

namespace River
{

    GLshort *LayerContext::Entry::buffer_quad(GLshort *buffer, int x, int y, int width, int height)
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

    GLfloat *LayerContext::Entry::buffer_coords(GLfloat *buffer, GLfloat x, GLfloat y, GLfloat x2, GLfloat y2)
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
	
	LayerContext::LayerContext(MemoryPool &memory_pool) : map(2, false, memory_pool), memory_pool(memory_pool)
	{
	}

	Layer *LayerContext::render()
	{
		ContentMeasurer measurer;

		measurer.count_objects<size_t>(map.get_entries());

		for(EntryMap::Iterator i = map.begin(); i != map.end(); ++i)
			(*i)->measure(measurer);

		void *memory = std::malloc(measurer.get_size());
			
		ContentSerializer serializer(memory);

		for(EntryMap::Iterator i = map.begin(); i != map.end(); ++i)
		{
			(*i)->serialize(serializer);

			// TODO: Add a conditional for debug test
			size_t &magic = serializer.write_object<size_t>();
			magic = 0xBEEF;
		}

		assert(serializer.get_position() - (char *)memory == measurer.get_size());
		
		return new Layer(memory, measurer.get_size());
	}

	LayerContext::Entry *LayerContext::lookup(uint32_t entry_type)
	{
		return map.get(entry_type);
	}

	void LayerContext::store(Entry *content)
	{
		map.set(content->entry_type, content);
	}
};
