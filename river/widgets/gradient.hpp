#pragma once
#include "../color.hpp"
#include "../layout/element.hpp"

namespace River
{
	class Gradient:
		public Element
	{
	private:
		bool is_horizontal;
		color_t colors[2];

	public:
		void vertical(color_t top, color_t bottom);
		void horizontal(color_t left, color_t right);

		void place(LayerCanvas *layer, int x, int y);
	};

};
