#pragma once
#include "object-list.hpp"

namespace River
{
	void ObjectList::render()
	{
		for(auto i = begin(); i; i++)
			i().render();
	}
};
