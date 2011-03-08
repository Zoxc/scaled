#pragma once
#include "block.hpp"
#include "../river.hpp"

namespace River
{
	class Stack:
		public Block
	{
	private:
		Orientation orientation;

		template<Orientation orientation> void generic_layout(int available_primary, int available_secondary);
	public:
		Stack();

		void set_orientation(Orientation orientation);
		void layout(int available_width, int available_height);
	};
};
