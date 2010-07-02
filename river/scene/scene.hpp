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
		extern RIVER_LIST(Window, scene_entry) windows;

		extern RIVER_LIST(State, scene_entry) states;
		extern size_t state_count;
		extern State **state_array;

		extern GradientState gradient_state;
		
		void render();
		void alloc();
		void free();
	};
};
