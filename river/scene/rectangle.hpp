#pragma once
#include "../gles.hpp"
#include "object.hpp"

namespace River
{
	class Rectangle:
		public Object
	{
	public:
		#pragma pack(push, 1)
		struct Point {
			GLshort x;
			GLshort y;
		};
		#pragma pack(pop)

		Point quad[4];

		void position(int x, int y, int width, int height);

		void render();
	};
};
