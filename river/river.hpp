#pragma once
#include <stddef.h>

namespace River
{
	enum Orientation
	{
		Horizontal,
		Vertical
	};

	enum MouseEvent
	{
		MouseEventUp,
		MouseEventDown,
		MouseEventMove,
		MouseEventEnter,
		MouseEventLeave,
	};

	size_t center(size_t object_height, size_t container_height);
};

#include "color.hpp"
