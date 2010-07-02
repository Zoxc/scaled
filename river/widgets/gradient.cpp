#pragma once
#include "gradient.hpp"

namespace River
{
	void Gradient::layout(int available_width, int available_height)
	{
		rect.width = width == Flags::Auto ? 20 : width;
		rect.height = height == Flags::Auto ? 20 : height;
	}

	void Gradient::place(Layer *layer, int x, int y)
	{
		object.position(x, y, rect.width, rect.height);
		layer->gradient_object_list.append(&object);
	}
};
