#include <algorithm>
#include "extends.hpp"

namespace River
{
	const Extends::Accessors Extends::accessors[2] = {
		{&Extends::left, &Extends::right, &Extends::top, &Extends::bottom},
		{&Extends::top, &Extends::bottom, &Extends::left, &Extends::right}
	};
	
	Extends::Extends(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom)
	{
	}

	Extends::Extends(const Extends *other)
	{
		*this = *other;
	}
	
	void Extends::union_with(const Extends *other)
	{
		left = std::max(left, other->left);
		top = std::max(top, other->top);
		right = std::max(right, other->right);
		bottom = std::max(bottom, other->bottom);
	}
};
