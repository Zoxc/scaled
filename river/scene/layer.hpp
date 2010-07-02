#pragma once
#include "../list.hpp"
#include "object-list.hpp"

namespace River
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		Entry<Layer> window_entry;

		ObjectList gradient_object_list;

		void render();
	};
};
