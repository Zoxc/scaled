#include <stdio.h>
#include <swl.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "gles-utils.hpp"
#include "river/layout/block.hpp"
#include "river/scene/scene.hpp"
#include "river/scene/gradient-object.hpp"

using namespace River;

Block hello;
Element test;
Layer *layer;
GradientObject *quad;

int main(void)
{
	enum swl_result result = swl_init("Test", 800, 480);

	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}
	
	hello.children.append(&test);

	Scene::alloc();

	layer = new Layer();

	quad = GradientObject::create_vertical(0, 0xFFFFFF);

	quad->position(10, 10, 50, 50);

	Scene::layers.append(layer);

	layer->object_lists[Scene::gradient_state.index].append(quad);
	
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
