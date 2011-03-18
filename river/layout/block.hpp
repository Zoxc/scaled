#pragma once
#include "../list.hpp"
#include "element.hpp"

namespace River
{
	class Block:
		public Element
	{
	public:
		Block() : block_flags(BlockNoFlags), padding(&no_padding) {}
		
		static Extends no_padding;

		typedef SimpleList<Element, Element, &Element::extend_entry> ExtendList;

		enum BlockFlags
		{
			BlockNoFlags = 0,
			BlockOwnsPadding = 1,
		};

		BlockFlags block_flags;
		Extends *padding;

		typedef List<Element, Element, &Element::children_entry> ChildrenList;

		ChildrenList children;

		void place(LayerCanvas *layer, int x, int y);
		bool mouse_outside(int x, int y);
		void mouse_event(MouseEvent event, int x, int y);
	};
};
