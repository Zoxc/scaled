#pragma once
#include "../gles.hpp"
#include "../list.hpp"

namespace River
{
	class Object
	{
	public:
		Entry<Object> entry;

		#pragma pack(push, 1)
		struct Point {
			GLshort x;
			GLshort y;
		};
		#pragma pack(pop)

		Point quad[4];

		void position(int x, int y, int width, int height);

		virtual void render() = 0;
	};
};
