#include "layer-context.hpp"
#include "layer.hpp"
#include "content-serializer.hpp"

namespace River
{
	Layer *LayerContext::render()
	{
		ContentMeasurer measurer;

		for(LayerCanvasList::Iterator i = layers.begin(); i != layers.end(); ++i)
			(*i)->measure(measurer);

		void *memory = std::malloc(measurer.get_size());
			
		ContentSerializer serializer(memory);

		for(LayerCanvasList::Iterator i = layers.begin(); i != layers.end(); ++i)
			(*i)->serialize(serializer);

		assert((size_t)(serializer.get_position() - (char *)memory) == measurer.get_size());
		
		return new Layer(memory, measurer.get_size());
	}

	LayerCanvas *LayerContext::add_layer()
	{
		LayerCanvas *layer = new (memory_pool) LayerCanvas(this, memory_pool);
		layers.append(layer);
		return layer;
	}
};
