#include <stdio.h>
#include <swl.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "gles-utils.hpp"
#include "river/layout/block.hpp"
#include "river/scene/scene.hpp"

River::Block hello;
River::Element test;

int main(void)
{
	enum swl_result result = swl_init("Test", 800, 480);

	hello.children.append(&test);
	
	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}
	
	char* vertex_source = "precision lowp float;\
		attribute vec2 point;\
		void main(void)\
		{\
			gl_Position.x = point.x / 400.0 - 1.0;\
			gl_Position.y = -(point.y / 240.0 - 1.0);\
		}";
	
	char* fragment_source = "precision lowp float;\
		uniform vec4 color;\
		void main (void)\
		{\
			gl_FragColor = color;\
		}";

	GLuint program = glCreateProgram();
	
	gluCompileShader(program, GL_VERTEX_SHADER, vertex_source);
	gluCompileShader(program, GL_FRAGMENT_SHADER, fragment_source);
	
	glBindAttribLocation(program, 0, "point");

	gluLinkProgram(program);
	
	GLuint color_uniform = glGetUniformLocation(program, "color");

	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glUseProgram(program);
	
	GLuint vbo;
	
	glGenBuffers(1, &vbo);
	
	GLshort vertices[] = {
		100, 164,
		100, 100,
		164, 164,
		164, 100,
		164, 100,
		
		200, 264,
		200, 264,
		200, 200,
		264, 264,
		264, 200
		};

	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glUniform4f(color_uniform, 1.0, 0.5, 1.0, 0.5);
	
	struct swl_event event;
	
	while(1)
	{
		while(swl_query(&event))
		{
			if(event.type == SWLE_QUIT)
				goto quit;
		}
		
		glClearColor(0.0f, 0.0f, (1 + sin(GetTickCount() / 1000.0f)) / 3, 1.0f);
		glUniform4f(color_uniform, 1.0f, 0.3f, (1 + sin(GetTickCount() / 1000.0f)) / 3, 0.5f);
		
		glClear(GL_COLOR_BUFFER_BIT);
	
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
	
		swl_swap();
	}

	quit:
	swl_quit();
	
	return 0;
}
