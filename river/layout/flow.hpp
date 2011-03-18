#pragma once
#include "container.hpp"

namespace River
{
	class Flow:
		public Container
	{
	public:
		void layout(int available_width, int available_height);
	};
};
