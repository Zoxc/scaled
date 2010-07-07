#pragma once
#include "../list.hpp"
#include "../layout/window.hpp"
#include "layer.hpp"
#include "state.hpp"
#include "gradient-state.hpp"

namespace River
{
	namespace Scene
	{
		typedef RIVER_LIST(Window, scene_entry) WindowList;

		extern WindowList windows;
		
		extern GradientState gradient_state;
		
		void render();
		void alloc();
		void size(int width, int height);
		void free();
	};
};
