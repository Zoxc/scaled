#include "gradient.hpp"
#include "../scene/gradient-canvas.hpp"

namespace River
{
	void Gradient::vertical(color_t top, color_t bottom)
	{
		colors[0] = top;
		colors[1] = bottom;
		is_horizontal = true;
	}

	void Gradient::horizontal(color_t left, color_t right)
	{
		colors[0] = left;
		colors[1] = right;
		is_horizontal = false;
	}

	void Gradient::place(LayerCanvas *layer, int x, int y)
	{
		GradientCanvas *gradient_canvas = GradientCanvas::acquire(layer);

		if(is_horizontal)
			gradient_canvas->render_horizontal(layer, x, y, rect.width, rect.height, colors[0], colors[1]);
		else
			gradient_canvas->render_vertical(layer, x, y, rect.width, rect.height, colors[0], colors[1]);
	}
};
