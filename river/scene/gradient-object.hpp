#pragma once
#include <stdint.h>
#include "../../gles-utils.hpp"
#include "object.hpp"

namespace River
{
	class GradientObject:
		public Object
	{
	private:
		struct Color {
			GLubyte r;
			GLubyte g;
			GLubyte b;

			Color &operator =(uint32_t color);
		};

		Color colors[4];
	public:
		static GradientObject *create_horizontal(uint32_t top, uint32_t bottom);
		static GradientObject *create_vertical(uint32_t left, uint32_t right);

		void render();
	};
};
