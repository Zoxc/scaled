#pragma once
#include "../list.hpp"
#include "element.hpp"

namespace River
{
	class Block:
		public Element
	{
	public:
		RIVER_LIST(Element, children_entry) children;
	};
};
