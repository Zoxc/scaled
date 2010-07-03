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
		
		typedef RIVER_LIST(State, scene_entry) StateList;

		extern StateList states;
		extern size_t state_count;
		extern State **state_array;

		extern GradientState gradient_state;
		
		void render();
		void alloc();
		void free();
	};
};
