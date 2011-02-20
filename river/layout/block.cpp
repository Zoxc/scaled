#include "block.hpp"

namespace River
{
	Extends Block::no_padding(0, 0, 0, 0);
	
	void Block::place(LayerContext *layer, int x, int y)
	{
		for(ChildrenList::Iterator i = children.begin(); i != children.end(); i++)
			i().place(layer, i().rect.left + x, i().rect.top + y);
	}
};
