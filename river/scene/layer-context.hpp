#pragma once
#include <stdint.h>
#include "../hash-table.hpp"
#include "../gles.hpp"
#include "../memory-pool.hpp"
#include "../simple-list.hpp"
#include "layer-canvas.hpp"

namespace River
{
	class Layer;

	class ContentMeasurer;
	class ContentSerializer;

	class LayerContext
	{
	private:
		MemoryPool memory_pool;
		typedef SimpleList<LayerCanvas> LayerCanvasList;

	public:
		Layer *render();

		LayerCanvasList layers;

		LayerCanvas *add_layer();
	};
};
