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
Extends padding(10, 10, 10, 10);
Extends test(20, 20, 20, 20);

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

	printf("GL extensions: %s\n", glGetString(GL_EXTENSIONS));
	
	Scene::windows.append(&win);
	Scene::raise_errors();

	FontSize *font = Scene::basic_font.get_size(12);

	struct swl_event event;
	
	int anim = 0;
	int anim_delta = 1;
	
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

		{
			River::Window::LayerList::Iterator i = win.layers.begin();

			while(i !=  win.layers.end())
			{
				Layer *layer = *i;
				++i;
				win.layers.remove(layer);
				delete layer;
			}
		}
		
		{
			MemoryPool memory_pool;
			LayerContext layer_context(memory_pool);
			LayerContext back_layer_context(memory_pool);
		
			GradientContext *gradient_context = GradientContext::acquire(&back_layer_context);
			GlyphContext *glyph_context = GlyphContext::acquire(&layer_context);

			size_t top_bar = 36;

			anim += anim_delta;

			if(anim < 0 || anim > 50)
				anim_delta *= -1;

			for(int i = 0; i < anim; ++i)
				glyph_context->render_glyph(&layer_context, 100 + i * 5, 120, font->get_glyph('.'), 0, color_white); 

			gradient_context->render_vertical(&layer_context, 0, 0, width, top_bar, 0x1a1c1a, 0x1a1c1a);
			gradient_context->render_vertical(&layer_context, 0, top_bar, width, height - top_bar, 0x232b24, 0x2d332e);
		
			glyph_context->render_text(&layer_context, 10, 23, "Launch Application", font, color_white);
			glyph_context->render_text(&layer_context, 590, 23, "20:32", font, color_white);

			glyph_context->render_text(&layer_context, 100, 200, "Hello there, this is just a bunch of text to stress the GPU a little.", font, color_white);
			glyph_context->render_text(&layer_context, 100, 240, "And here is some more. Please don't waste time reading this.", font, color_white);
			glyph_context->render_text(&layer_context, 100, 280, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", font, color_white);
			glyph_context->render_text(&layer_context, 100, 320, "Etiam ut nibh felis. Duis lectus arcu, vestibulum vel ullamcorper sed, varius sed est.", font, color_white);
			glyph_context->render_text(&layer_context, 100, 360, "Donec accumsan, justo in sodales pharetra, ipsum lacus tincidunt nisi, sed tristique nisi sapien in dui.", font, color_white);
			/*
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
			*/
			win.layers.append(back_layer_context.render());
			win.layers.append(layer_context.render());
		}
	
		Scene::render();

		swl_swap();
		frame();
	}

	Scene::free();

	quit:
	swl_quit();
	
	return 0;
}
