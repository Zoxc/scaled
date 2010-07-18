#include "rectangle.hpp"

namespace River
{
	void Rectangle::position(int x, int y, int width, int height)
	{
		int right = x + width;
		int bottom = y + height;
		
		quad[0].x = x;
		quad[0].y = y;
		
		quad[1].x = right;
		quad[1].y = y;

		quad[2].x = x;
		quad[2].y = bottom;

		quad[3].x = right;
		quad[3].y = bottom;
	}

	void Rectangle::render()
	{
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, quad);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		Object::render();
	}
};
