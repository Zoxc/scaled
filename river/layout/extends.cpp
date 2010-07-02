#pragma once
#include <algorithm>
#include "extends.hpp"

namespace River
{
	Extends::Extends(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom)
	{
	}

	Extends::Extends(int left, int top, int right, int bottom, Extends *margins)
	{
		this->left = std::max(left, margins->left);
		this->top = std::max(top, margins->top);
		this->right = std::max(right, margins->right);
		this->bottom = std::max(bottom, margins->bottom);
	}
};
