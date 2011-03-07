#pragma once
#include <stdint.h>

namespace River
{
	typedef uint32_t color_t;

	uint8_t color_red_component(color_t color);
	uint8_t color_green_component(color_t color);
	uint8_t color_blue_component(color_t color);
	uint8_t color_alpha_component(color_t color);
	
	const color_t color_black = 0x000000FF;
	const color_t color_white = 0xFFFFFFFF;

	color_t mix_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	color_t mix_color_alpha(color_t input, uint8_t alpha);
};
