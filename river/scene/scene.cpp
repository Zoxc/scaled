#include "scene.hpp"
#include "fonts/font.hpp"

namespace River
{
	namespace Scene
	{
		RIVER_LIST(Window, scene_entry) windows;

		RIVER_LIST(State, scene_entry) states;
		size_t state_count;
		State **state_array;

		GradientState gradient_state;
		GlyphState glyph_state;
		
		Font basic_font;

		void render()
		{
			for(WindowList::Iterator i = windows.begin(); i; i++)
				i().render();
		}

		void size(int width, int height)
		{
			glViewport(0, 0, width, height);
			
			gradient_state.size(width, height);
			glyph_state.size(width, height);
		}

		void alloc()
		{
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			Font::setup();

			if(!basic_font.allocate("fonts/DroidSans.ttf"))
				printf("Unable to alllocate basic font.\n");
			
			gradient_state.alloc();
			glyph_state.alloc();
		}

		void free()
		{
			gradient_state.free();
			glyph_state.free();
		}
	};
};
