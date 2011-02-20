#include <stdio.h>
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
#include "river/scene/gradient-object.hpp"
#include "river/scene/fonts/text-object.hpp"
#include "river/scene/fonts/glyph-context.hpp"
#include "river/scene/fonts/glyph.hpp"

#ifndef WIN32
	#include <sys/time.h>
#endif

using namespace River;

Gradient gradient1;
Gradient gradient2;
River::Window win;
Layer *layer;
Extends padding(10, 10, 10, 10);
GradientObject *quad;
TextObject text_object;
TextObject fps;
TextObject text_object2;
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

void frame()
{
	static size_t frames;
	static size_t last_update;

	frames++;

	if(get_ticks() - last_update > 250)
	{
		std::stringstream caption;

		caption << frames / ((get_ticks() - last_update) / 1000.f) << " fps";

		fps.position(550, 70, font, color_black, caption.str().c_str());
		fps.attach(layer);

		last_update = get_ticks();

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

		GlyphContext *glyph_context = GlyphContext::acquire(&layer_context);

		glyph_context->render_glyph(&layer_context, 10, 10, font->get_glyph('A'), 0, color_black);

	/*
		text_object.position(100, 200, font, color_black, "Hello there, this is just a bunch of text to stress the GPU a little.");
		text_object.attach(layer);
	
		text_object2.position(100, 220, font, color_black, "And here is some more. Please don't waste time reading this.");
		text_object2.attach(layer);
	*/
		gradient1.object.vertical(0xFF3412, 0x23FF12);
		gradient1.width = Element::Flags::Extend;
		gradient1.height = 5;
		gradient1.margins = &test;

		gradient2.object.horizontal(0xFF3412, 0x23FF12);
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
		
		//frame();
		Scene::render();

		swl_swap();
	}

	delete layer;
	delete quad;

	Scene::free();

	quit:
	swl_quit();
	
	return 0;
}
