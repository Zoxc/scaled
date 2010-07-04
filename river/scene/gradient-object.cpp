#include "gradient-object.hpp"
#include <stdio.h>

namespace River
{
	GradientObject::Color &GradientObject::Color::operator =(uint32_t color)
	{
		r = (color & 0xFF) / 255.0;
		g = ((color >> 8) & 0xFF) / 255.0;
		b = ((color >> 16) & 0xFF) / 255.0;

		return *this;
	}

	void GradientObject::horizontal(uint32_t top, uint32_t bottom)
	{
		colors[0] = top;
		colors[1] = top;
		colors[2] = bottom;
		colors[3] = bottom;
	}

	void GradientObject::vertical(uint32_t left, uint32_t right)
	{
		colors[0] = left;
		colors[1] = right;
		colors[2] = left;
		colors[3] = right;
	}

	void GradientObject::render()
	{
		printf("quad: {\n\t{%f, %f},\n\t{%f, %f},\n\t{%f, %f},\n\t{%f, %f}\n}\n", quad[0].x, quad[0].y, quad[1].x, quad[1].y, quad[2].x, quad[2].y, quad[3].x, quad[3].y);
		printf("colors: {\n\t{%f, %f, %f},\n\t{%f, %f, %f},\n\t{%f, %f, %f},\n\t{%f, %f, %f}\n}\n", colors[0].r, colors[0].g, colors[0].b, colors[1].r, colors[1].g, colors[1].b, colors[2].r, colors[2].g, colors[2].b, colors[3].r, colors[3].g, colors[3].b);
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, quad);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colors);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};
