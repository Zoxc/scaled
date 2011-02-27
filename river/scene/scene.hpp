#pragma once
#include "../list.hpp"
#include "../layout/window.hpp"
#include "fonts/font.hpp"
#include "fonts/glyph-state.hpp"
#include "layer.hpp"
#include "state.hpp"
#include "gradient-state.hpp"
#include "colored-image-canvas.hpp"

namespace River
{
	namespace Scene
	{
		typedef List<Window, Window, &Window::scene_entry> WindowList;

		extern WindowList windows;

		extern Font basic_font;
		
		extern ColoredImageState colored_image_state;
		extern GradientState gradient_state;
		extern GlyphState glyph_state;
		
		void render();
		void alloc();
		void size(int width, int height);
		void raise_errors();
		void free();

		size_t get_draw_calls();
		void draw_call();
	};
};
