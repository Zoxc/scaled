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
			for(WindowList::Iterator i = windows.begin(); i; i++)
				i().render();
		}

		void size(int width, int height)
		{
			glViewport(0, 0, width, height);

			gradient_state.size(width, height);
		}

		void alloc()
		{
			gradient_state.alloc();
		}

		void free()
		{
			gradient_state.free();
		}
	};
};
