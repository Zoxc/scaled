#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <swl.h>
#include "river/color.hpp"
#include "river/image.hpp"
#include "river/layout/block.hpp"
#include "river/layout/stack.hpp"
#include "river/layout/background.hpp"
#include "river/widgets/gradient.hpp"
#include "river/widgets/label.hpp"
#include "river/scene/scene.hpp"
#include "river/scene/layer-context.hpp"
#include "river/scene/layer-canvas.hpp"
#include "river/scene/fonts/glyph-canvas.hpp"
#include "river/scene/gradient-canvas.hpp"
#include "river/scene/fonts/glyph.hpp"
#include "river/scene/fonts/font-size.hpp"
#include "window-state.hpp"
#include "launcher/widgets/category.hpp"
#include "launcher/widgets/icon.hpp"

#ifndef WIN32
	#include <sys/time.h>
#endif

using namespace River;
using namespace Launcher;

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

Gradient top_bar;
Gradient background;
Label title, clock_label;
Flow top_flow, bottom_flow, bottom_icons;
Extends title_margins(12, 12, 12, 12);
Extends cat_margins(8, 8, 8, 8);
Stack top_element, bottom_stack;
Background top, bottom;
River::Window win;
Extends padding(10, 10, 10, 10);
Extends test(20, 20, 20, 20);

const int width = 800;
const int height = 480;


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

	Atlas<GL_RGBA> icon_atlas;

	{
		MemoryPool memory_pool;
		LayerContext context;

		std::vector<std::string> categories;
		
		categories.push_back("Applications");
		categories.push_back("Communication");
		categories.push_back("Media");
		categories.push_back("Internet");
		categories.push_back("Utilities");
		categories.push_back("Configuration");

		for(size_t i = 0; i < categories.size(); ++i)
		{
			Image *image = new Image(&icon_atlas);
			image->load_png("icons/" + categories[i] + ".png");

			CategoryWidget *widget = new CategoryWidget();
			widget->set_title(categories[i]);
			widget->margins = &cat_margins;
			widget->set_icon(image);
			bottom_stack.children.append(widget);
		}
		
		for(size_t i = 1; i <= 27; ++i)
		{
			std::stringstream name;
			name << i;
			Image *image = new Image(&icon_atlas);
			image->load_png("icons/apps/app (" + name.str() + ").png");

			IconWidget *widget = new IconWidget();
			widget->set_title("App " + name.str());
			widget->margins = &cat_margins;
			widget->set_icon(image);
			bottom_icons.children.append(widget);
		}
		
		title.margins = &title_margins;
		title.set_caption("Launch Application");
		title.set_color(0xcdcdcdff);
		title.width = Element::Flags::Extend;
		
		clock_label.margins = &title_margins;
		clock_label.set_caption("20:34");
		clock_label.set_color(0xcdcdcdff);




		top_flow.children.append(&title);
		top_flow.children.append(&clock_label);

		
		top.width = Element::Flags::Extend;
		top.set_content(&top_flow);

		bottom.width = Element::Flags::Extend;
		bottom.height = Element::Flags::Extend;
		bottom.set_content(&bottom_flow);
		
		bottom_flow.children.append(&bottom_stack);
		bottom_flow.children.append(&bottom_icons);

		top_element.children.append(&top);
		top_element.children.append(&bottom);
		
		top_bar.vertical(0x1a1c1aff, 0x1a1c1aff);

		top.set_background(&top_bar);

		background.vertical(0x2d332eff, 0x232b24ff);

		bottom.set_background(&background);

		win.element.children.append(&top_element);
		win.element.layout(width, height);
		win.element.place(context.add_layer(), 0, 0);
		
		win.layers.append(context.render());
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
				
			case SWLE_KEYDOWN:
				if(event.key_event.key == SWLK_GO)
					goto quit;
				break;
				
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
