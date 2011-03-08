#pragma once
#include <stddef.h>

namespace River
{
	enum Orientation
	{
		Horizontal,
		Vertical
	};

	size_t center(size_t object_height, size_t container_height);
};

#include "color.hpp"
