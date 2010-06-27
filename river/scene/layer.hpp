#pragma once
#include "../list.hpp"
#include "object.hpp"

namespace River
{
	class ObjectList
	{
	public:
		RIVER_LIST(Object, list_entry) list;
	};

	class Layer
	{
	public:
		Entry<Layer> scene_entry;
		ObjectList *object_lists;
	};
};
