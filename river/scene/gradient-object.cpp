#include "gradient-object.hpp"
#include "layer.hpp"

namespace River
{
	GradientObject::Color &GradientObject::Color::operator =(uint32_t color)
	{
		r = color & 0xFF;
		g = (color >> 8) & 0xFF;
		b = (color >> 16) & 0xFF;

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
		glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors);

		Rectangle::render();
	}

	void GradientObject::place(Layer *layer)
	{
		layer->gradient_object_list.append(this);
	}
};
