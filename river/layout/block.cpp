#include "block.hpp"

namespace River
{
	Extends Block::no_padding(0, 0, 0, 0);
	
	void Block::place(LayerCanvas *layer, int x, int y)
	{
		for(ChildrenList::Iterator i = children.begin(); i != children.end(); i++)
			i().place(layer, i().rect.left + x, i().rect.top + y);
	}
	
	bool Block::mouse_outside(int x, int y)
	{
		if(Element::mouse_outside(x, y))
		{
			flags &= ~Flags::MouseOver;
			
			for(ChildrenList::Iterator i = children.begin(); i != children.end(); ++i)
			{
				int child_x = x - i().rect.left;
				int child_y = y - i().rect.top;

				i().mouse_outside(child_x, child_y);
			}
			
			return true;
		}
		else
			return false;
	}

	void Block::mouse_event(MouseEvent event, int x, int y)
	{
		mouse_inside(x, y);

		for(ChildrenList::Iterator i = children.begin(); i != children.end(); ++i)
		{
			if(i().flags & Element::Flags::Visible)
			{
				int child_x = x - i().rect.left;
				int child_y = y - i().rect.top;

				if(i().inside(child_x, child_y))
				{
					i().mouse_event(event, child_x, child_y);

					// Tell the remaining children that they are outside
					 
					while(i != children.end())
					{
						int child_x = x - i().rect.left;
						int child_y = y - i().rect.top;
				
						i().mouse_outside(child_x, child_y);

						++i;
					}

					return;
				}
				else
					i().mouse_outside(child_x, child_y);
			}
		}

		// The event didn't match any children, it's for us

		Element::mouse_event(event, x, y);
	}
};
