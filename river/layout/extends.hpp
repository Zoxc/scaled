#pragma once
#include "../list.hpp"

namespace River
{
	struct Extends
	{
	public:
		Extends(int left, int top, int right, int bottom);
		Extends(int left, int top, int right, int bottom, Extends *margins);

		int left;
		int top;
		int right;
		int bottom;
	};
};
