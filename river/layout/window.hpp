#pragma once
#include "../list.hpp"
#include "../scene/layer.hpp"
#include "flow.hpp"

namespace River
{
	class Window
	{
	public:
		Flow element;
		Entry<Window> scene_entry;

		typedef List<Layer, Layer, &Layer::window_entry> LayerList;

		LayerList layers;

		void render();
	};

};
