#pragma once
#include "../list.hpp"
#include "object.hpp"

namespace River
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		typedef RIVER_LIST(Object, list_entry) ObjectList;

		Entry<Layer> scene_entry;
		ObjectList *object_lists;

		void render();
	};
};
