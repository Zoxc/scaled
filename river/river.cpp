#include "river.hpp"

namespace River
{
	size_t center(size_t object_height, size_t container_height)
	{
		return (container_height - object_height) >> 1;
	}
};
