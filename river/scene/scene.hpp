#pragma once
#include "../list.hpp"
#include "layer.hpp"
#include "state.hpp"

namespace River
{
	namespace Scene
	{
		RIVER_LIST(Layer, scene_entry) layers;
		size_t state_count;
		State *state_array;
		RIVER_LIST(State, scene_entry) states;
		
		void setup();
		void setup_states();
		void render();
	};
};
