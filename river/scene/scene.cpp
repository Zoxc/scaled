#pragma once
#include "scene.hpp"

namespace River
{
	namespace Scene
	{
		RIVER_LIST(Window, scene_entry) windows;

		RIVER_LIST(State, scene_entry) states;
		size_t state_count;
		State **state_array;

		GradientState gradient_state;
		
		void render()
		{
			for(auto i = windows.begin(); i; i++)
				i().render();
		}

		void alloc()
		{
			state_count = State::StateCount;

			for(auto i = states.begin(); i; i++)
			{
				state_count++;
			}

			state_array = new State *[state_count];

			state_array[State::GradientState] = &gradient_state;

			int j = State::StateCount;
			for(auto i = states.begin(); i; i++, j++)
			{
				i().index = j;
				state_array[i] = &(i());
			}

			for(size_t i = 0; i < state_count; i++)
				state_array[i]->alloc();
		}

		void free()
		{
			for(auto i = states.begin(); i; i++)
				i().free();
		}
	};
};
