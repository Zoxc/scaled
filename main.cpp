#include <stdio.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "swl/swl.h"
#include "river/layout/block.hpp"
#include "river/widgets/gradient.hpp"
#include "river/scene/scene.hpp"
#include "river/scene/gradient-object.hpp"
#include "river/scene/fonts/glyph-object.hpp"
#include "river/scene/fonts/glyph.hpp"

using namespace River;

Gradient gradient1;
Gradient gradient2;
River::Window win;
Layer *layer;
Extends padding(10, 10, 10, 10);
GradientObject *quad;
GlyphObject glyph_object;
Extends test(20, 20, 20, 20);

const int width = 640;
const int height = 400;

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

	layer = new Layer();

	FontSize *font = Scene::basic_font.get_size(12);

	font->ref();

	Glyph *glyph = font->get_glyph('A');

	glyph_object.position(100, 200, glyph->width, glyph->height);
	glyph_object.set_glyph(glyph, 0xFF000000);
	glyph_object.place(layer);
	
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
	win.element.place(layer, 0, 0);
	
	win.layers.append(layer);
	Scene::windows.append(&win);

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
				Scene::size(event.size_event.width, event.size_event.height);
				win.element.layout(event.size_event.width, event.size_event.height);
				win.element.place(layer, 0, 0);
				break;

			default:
				break;
			}
		}
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
