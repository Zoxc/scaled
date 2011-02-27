#include <iostream>
#include "scene.hpp"
#include "fonts/font.hpp"
#include "buffer.hpp"

#ifdef SWL_CONTEXT_EGL
	#include <EGL/egl.h>
#endif

namespace River
{
	namespace Scene
	{
		WindowList windows;

		ColoredImageState colored_image_state;
		GradientState gradient_state;
		GlyphState glyph_state;
		
		Font basic_font;

		void render()
		{
			for(WindowList::Iterator i = windows.begin(); i != windows.end(); i++)
				i().render();
		}

		void size(int width, int height)
		{
			glViewport(0, 0, width, height);
			
			colored_image_state.size(width, height);
			gradient_state.size(width, height);
			glyph_state.size(width, height);
		}

		void raise_errors()
		{
			GLenum gl_error = glGetError();

			if(gl_error)
			{
				std::cout << "glGetError returned " << gl_error << "\n";
				assert(0);
			}
		}

		void alloc()
		{
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			Buffer::setup();

			Font::setup();

			if(!basic_font.allocate("fonts/DroidSans.ttf"))
				printf("Unable to alllocate basic font.\n");
			
			colored_image_state.alloc();
			gradient_state.alloc();
			glyph_state.alloc();
		}

		void free()
		{
			colored_image_state.free();
			gradient_state.free();
			glyph_state.free();
		}

		static size_t draw_calls;
		
		size_t get_draw_calls()
		{
			return draw_calls;
		}

		void draw_call()
		{
			draw_calls++;
		}
	};
};
