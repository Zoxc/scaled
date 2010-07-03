#include "object-list.hpp"

namespace River
{
	void ObjectList::render()
	{
		for(Iterator i = begin(); i; i++)
			i().render();
	}
};
