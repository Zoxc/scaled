#pragma once
#include <stdint.h>
#include "../gles.hpp"
#include "object.hpp"

namespace River
{
	class GradientObject:
		public Object
	{
	private:
		#pragma pack(push, 1)
		struct Color {
			GLubyte r;
			GLubyte g;
			GLubyte b;

			Color &operator =(uint32_t color);
		};
		#pragma pack(pop)

		Color colors[4];
	public:
		void horizontal(uint32_t top, uint32_t bottom);
		void vertical(uint32_t left, uint32_t right);

		void render();
	};
};
