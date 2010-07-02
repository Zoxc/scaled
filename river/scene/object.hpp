#pragma once
#include "../../gles-utils.hpp"
#include "../list.hpp"

namespace River
{
	class Object
	{
	public:
		Entry<Object> entry;

		struct Point {
			GLshort x;
			GLshort y;
		};

		Point quad[4];

		void position(int x, int y, int width, int height);

		virtual void render() = 0;
	};
};
