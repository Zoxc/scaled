#pragma once
#include "block.hpp"

namespace River
{
	Extends Block::no_padding(0, 0, 0, 0);
	
	void Block::place(Layer *layer, int x, int y)
	{
		for(auto i = children.begin(); i; i++)
			i().place(layer, i().rect.left + x, i().rect.top + y);
	}
};
