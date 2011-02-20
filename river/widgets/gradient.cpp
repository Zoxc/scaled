#include "gradient.hpp"

namespace River
{
	void Gradient::place(LayerContext *layer, int x, int y)
	{
		object.position(x, y, rect.width, rect.height);
		object.place(layer);
	}
};
