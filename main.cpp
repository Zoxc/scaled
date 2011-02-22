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
#include "river/scene/gradient-object.hpp"
#include "river/scene/fonts/glyph-context.hpp"
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
GradientObject *quad;
Extends test(20, 20, 20, 20);
FontSize *font;
WindowState window_state;

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

class TestWindow
{
public:
	Layer *layer;
	GLuint fbo;
	GLuint tex;
	float scale;
	int x;

	TestWindow(int x, float scale) : scale(scale), x(x)
	{
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &tex);
	
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	
		GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		assert(err == GL_FRAMEBUFFER_COMPLETE);
	
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		glViewport(0, 0, width, height);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		Scene::render();
	}

	void render_view()
	{
		glBindTexture(GL_TEXTURE_2D, tex);

		GLshort win[12];
		
		buffer_quad(win, x + (int)floor(sin(get_ticks() / 1000.0 * scale) * 30), 50 + (int)floor(sin(get_ticks() / 700.0 * scale) * 30), x + 300 + (int)floor(sin(get_ticks() / 200.0 * scale) * 30), 360 + (int)floor(sin(get_ticks() / 800.0 * scale) * 30));

		GLfloat tex[12];

		buffer_coords(tex, 0, 1, 1, 0);

		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, win);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, tex);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		Scene::draw_call();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
};

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
	
	window_state.alloc();
	window_state.size(width, height);
	
	GLint orginal_fbo;

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &orginal_fbo);
	
	TestWindow win1(50, 1.0);
	TestWindow win2(150, 1.5);
	TestWindow win3(250, 2.0);
	TestWindow win4(350, 2.5);
	
	{
		MemoryPool memory_pool;
		LayerContext layer_context(memory_pool);

		font = Scene::basic_font.get_size(9);

		GlyphContext *glyph_context = GlyphContext::acquire(&layer_context);
		
		for(int i = 0; i < 4; i++)
		{
			glyph_context->render_text(&layer_context, 100, 200 + i * 40, "Hello there, this is just a bunch of text to stress the GPU a little.", font, color_black);
			glyph_context->render_text(&layer_context, 100, 220 + i * 40, "And here is some more. Please don't waste time reading this.", font, color_black);
		}

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
		
		win1.render();
		win2.render();
		win3.render();
		win4.render();

		glBindFramebuffer(GL_FRAMEBUFFER, orginal_fbo);
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		window_state.use();
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		
		win1.render_view();
		win2.render_view();
		win3.render_view();
		win4.render_view();

		swl_swap();
		frame();
	}

	delete layer;
	delete quad;

	Scene::free();

	quit:
	swl_quit();
	
	return 0;
}
