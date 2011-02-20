#include "layer-context.hpp"
#include "layer.hpp"

namespace River
{
	LayerContext::LayerContext(MemoryPool &memory_pool) : memory_pool(memory_pool), map(2, false, memory_pool)
	{
	}

	Layer *LayerContext::render()
	{
		Layer *layer = new Layer;

		for(EntryMap::Iterator i = map.begin(); i != map.end(); ++i)
			(*i)->render(layer);

		return layer;
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
