#include "color.hpp"

namespace River
{
	uint8_t color_red_component(color_t color)
	{
		return color & 0xFF;
	}

	uint8_t color_green_component(color_t color)
	{
		return (color >> 8) & 0xFF;
	}

	uint8_t color_blue_component(color_t color)
	{
		return (color >> 16) & 0xFF;
	}

	uint8_t color_alpha_component(color_t color)
	{
		return (color >> 24) & 0xFF;
	}

	color_t color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		return red | (green << 8) | (blue << 16) | (alpha << 24);
	}
};
