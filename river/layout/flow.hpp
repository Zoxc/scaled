#pragma once
#include "block.hpp"

namespace River
{
	class Flow:
		public Block
	{
	private:
		struct Line
		{
			Element *start;

			/*
			 * This includes the top margin.
			 */
			int height;

			int margin_bottom;
		};
	public:
		void layout(int available_width, int available_height);
	};
};
