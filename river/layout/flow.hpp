#pragma once
#include "block.hpp"

namespace River
{
	class Flow:
		public Block
	{
	public:
		void layout(int available_width, int available_height);
	};
};
