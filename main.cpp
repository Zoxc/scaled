#include <stdio.h>
#include <swl.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "gles-utils.hpp"
#include "river/layout/block.hpp"
#include "river/widgets/gradient.hpp"
#include "river/scene/scene.hpp"
#include "river/scene/gradient-object.hpp"

using namespace River;

Gradient gradient;
Window win;
Layer *layer;
Extends padding(10, 10, 10, 10);
GradientObject *quad;

int main(void)
{
	enum swl_result result = swl_init("Test", 800, 480);

	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}
	
	Scene::alloc();

	layer = new Layer();

	gradient.object.vertical(0xFF3412, 0x23FF12);
	gradient.width = Element::Flags::Extend;

	win.element.padding = &padding;
	win.element.children.append(&gradient);
	win.element.layout(800, 480);
	win.element.place(layer, 0, 0);
	
	win.layers.append(layer);
	Scene::windows.append(&win);

	struct swl_event event;
	
	while(1)
	{
		while(swl_query(&event))
		{
			if(event.type == SWLE_QUIT)
				goto quit;
		}
		
		glClearColor(0.0f, 0.0f, (1 + sin(GetTickCount() / 1000.0f)) / 3, 1.0f);
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
