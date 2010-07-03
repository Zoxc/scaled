#include "gradient.hpp"

namespace River
{
	void Gradient::place(Layer *layer, int x, int y)
	{
		object.position(x, y, rect.width, rect.height);
		layer->gradient_object_list.append(&object);
	}
};
