#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "swl/swl.h"
#include "river/color.hpp"
#include "river/layout/block.hpp"
#include "river/widgets/gradient.hpp"
#include "river/scene/scene.hpp"
#include "river/scene/layer-context.hpp"
#include "river/scene/fonts/glyph-context.hpp"
#include "river/scene/gradient-context.hpp"
#include "river/scene/fonts/glyph.hpp"
#include "window-state.hpp"

#ifndef WIN32
	#include <sys/time.h>
#endif

using namespace River;

Gradient gradient1;
Gradient gradient2;
River::Window win;
Layer *layer;
Extends padding(10, 10, 10, 10);
Extends test(20, 20, 20, 20);
FontSize *font;

const int width = 640;
const int height = 400;

int get_ticks()
{
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval tv;

		if(gettimeofday(&tv, 0) != 0)
			return 0;

		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	#endif
}

GLshort *buffer_quad(GLshort *buffer, int x, int y, int x2, int y2)
{
    *buffer++ = x;
	*buffer++ = y;

    *buffer++ = x2;
	*buffer++ = y;

    *buffer++ = x;
	*buffer++ = y2;

    *buffer++ = x2;
	*buffer++ = y;

    *buffer++ = x;
	*buffer++ = y2;

    *buffer++ = x2;
	*buffer++ = y2;

    return buffer;
}

GLfloat *buffer_coords(GLfloat *buffer, int x, int y, int x2, int y2)
{
    *buffer++ = (GLfloat)x;
	*buffer++ = (GLfloat)y;
		
    *buffer++ = (GLfloat)x2;
	*buffer++ = (GLfloat)y;

    *buffer++ = (GLfloat)x;
	*buffer++ = (GLfloat)y2;
	
    *buffer++ = (GLfloat)x2;
	*buffer++ = (GLfloat)y;

    *buffer++ = (GLfloat)x;
	*buffer++ = (GLfloat)y2;

    *buffer++ = (GLfloat)x2;
	*buffer++ = (GLfloat)y2;

    return buffer;
}


void frame()
{
	static size_t frames;
	static size_t last_update;
	static size_t calls;

	frames++;

	if(get_ticks() - last_update > 1000)
	{
		std::stringstream fps_str;

		fps_str << frames / ((get_ticks() - last_update) / 1000.f) << " fps";

		std::cout << fps_str.str() << "\n";

		std::stringstream call_str;

		size_t call_count = Scene::get_draw_calls();

		call_str << (call_count - calls) / ((get_ticks() - last_update) / 1000.f) << " call/s";

		std::cout << call_str.str() << "\n";

		last_update = get_ticks();
		calls = call_count;

		frames = 0;
	}
}

int main(void)
{
	enum swl_result result = swl_init("scaled", width, height, true);

	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}

	Scene::alloc();
	Scene::size(width, height);
	
	{
		MemoryPool memory_pool;
		LayerContext layer_context(memory_pool);

		font = Scene::basic_font.get_size(9);

		GradientContext *gradient_context = GradientContext::acquire(&layer_context);
		GlyphContext *glyph_context = GlyphContext::acquire(&layer_context);

		gradient_context->render_horizontal(&layer_context, 0, 0, width, height, 0x232b24, 0x2d332e);

		glyph_context->render_text(&layer_context, 100, 200, "Hello there, this is just a bunch of text to stress the GPU a little.", font, color_black);
		glyph_context->render_text(&layer_context, 100, 220, "And here is some more. Please don't waste time reading this.", font, color_black);
		
		gradient1.vertical(0xFF3412, 0x23FF12);
		gradient1.width = Element::Flags::Extend;
		gradient1.height = 5;
		gradient1.margins = &test;

		gradient2.horizontal(0xFF3412, 0x23FF12);
		gradient2.width = Element::Flags::Extend;
		gradient2.height = Element::Flags::Extend;

		win.element.padding = &padding;
		win.element.children.append(&gradient1);
		win.element.children.append(&gradient2);
		win.element.layout(width, height);
		win.element.place(&layer_context, 0, 0);
		
		layer = layer_context.render();
		win.layers.append(layer);
	}
	
	Scene::windows.append(&win);
	
	Scene::raise_errors();

	struct swl_event event;
	
	while(1)
	{
		while(swl_query(&event))
		{
			switch(event.type)
			{
			case SWLE_QUIT:
				goto quit;

			case SWLE_RESIZE:
				/*Scene::size(event.size_event.width, event.size_event.height);
				win.element.layout(event.size_event.width, event.size_event.height);
				win.element.place(layer_context, 0, 0);*/
				break;

			default:
				break;
			}
		}
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		Scene::render();

		swl_swap();
		frame();
	}

	Scene::free();

	quit:
	swl_quit();
	
	return 0;
}
