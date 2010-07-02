#pragma once
#include "../layout/element.hpp"
#include "../scene/gradient-object.hpp"

namespace River
{
	class Gradient:
		public Element
	{
	public:
		GradientObject object;

		void layout(int available_width, int available_height);
		void place(Layer *layer, int x, int y);
	};

};
