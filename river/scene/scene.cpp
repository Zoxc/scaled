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

			#ifdef SWL_CONTEXT_EGL
				EGLint egl_error = eglGetError();

				if(egl_error != EGL_SUCCESS)
				{
					std::cout << "eglGetError returned " << egl_error << "\n";
					assert(0);
				}
			#endif
		}

		void alloc()
		{
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			Buffer::setup();

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
