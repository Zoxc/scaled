#pragma once
#include "gradient-object.hpp"

namespace River
{
	GradientObject::Color &GradientObject::Color::operator =(uint32_t color)
	{
		r = color & 0xFF;
		g = (color >> 8) & 0xFF;
		b = (color >> 16) & 0xFF;

		return *this;
	}

	GradientObject *GradientObject::create_horizontal(uint32_t top, uint32_t bottom)
	{
		GradientObject &result = *new GradientObject();
		
		result.colors[0] = top;
		result.colors[1] = top;
		result.colors[2] = bottom;
		result.colors[3] = bottom;

		return &result;
	}

	GradientObject *GradientObject::create_vertical(uint32_t left, uint32_t right)
	{
		GradientObject &result = *new GradientObject();
		
		result.colors[0] = left;
		result.colors[1] = right;
		result.colors[2] = left;
		result.colors[3] = right;

		return &result;
	}

	void GradientObject::render()
	{
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, quad);
		glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};
