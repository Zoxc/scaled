#pragma once
#include "../list.hpp"
#include "element.hpp"

namespace River
{
	class Block:
		public Element
	{
	public:
		Block() : padding(&no_padding), block_flags(BlockNoFlags) {}
		
		static Extends no_padding;

		typedef RIVER_LIST(Element, extend_entry) ExtendList;

		enum BlockFlags
		{
			BlockNoFlags = 0,
			BlockOwnsPadding = 1,
		};

		BlockFlags block_flags;
		Extends *padding;

		RIVER_LIST(Element, children_entry) children;

		void place(Layer *layer, int x, int y);
	};
};
