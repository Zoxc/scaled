#include <stdio.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "swl.h"
#include "gles.hpp"
#include "OpenGL.Texture.hpp"

const int width = 800;
const int height = 480;

int main(void)
{
	OpenGL::Texture tex;
	
	enum swl_result result = swl_init("blending", width, height, true);

	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}
	
	tex.Load("font.png");

	const char* fragment_shader =
		"precision highp float;\
		varying vec2 VCord;\
		uniform sampler2D Texture;\
		uniform vec4 Color;\
		void main (void)\
		{\
			vec4 mask = texture2D(Texture, VCord); \
			gl_FragColor = mask * Color.a;\
			gl_FragColor.a = 0.0;\
		}";

	const char* vertex_shader =
		"precision highp float;\
		attribute vec2 APoint;\
		attribute vec2 ACord;\
		varying vec2 VCord;\
		void main(void)\
		{\
			gl_Position.x = APoint.x / 400.0 - 1.0;\
			gl_Position.y = -(APoint.y / 240.0 - 1.0);\
			gl_Position.z = 0.0;\
			gl_Position.w = 1.0;\
			VCord = ACord;\
		}";

	GLuint program = glCreateProgram();
	
	gluCompileShader(program, GL_FRAGMENT_SHADER, fragment_shader);
	gluCompileShader(program, GL_VERTEX_SHADER, vertex_shader);
	
	glBindAttribLocation(program, 0, "APoint");
	glBindAttribLocation(program, 1, "ACord");

	gluLinkProgram(program);
	
	GLuint TextureUniform = glGetUniformLocation(program, "Texture");
	GLuint ColorUniform = glGetUniformLocation(program, "Color");
	
	glUseProgram(program);
	
	glUniform1i(TextureUniform, 0);
    glUniform4f(ColorUniform, 1, 0, 0, 1.0);
	
    glBindTexture(GL_TEXTURE_2D, tex.Handle);

	glEnable(GL_BLEND);

	glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBlendColor(1, 0, 1, 1.0);
	
	GLenum error = glGetError();
	
	if(error != GL_NO_ERROR)
	{
		printf("OpenGL failed with error 0x%x.\n", error);
		return 0;
	}
	
	glActiveTexture(GL_TEXTURE0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

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
				glViewport(0, 0, event.size_event.width, event.size_event.height);
				break;

			default:
				break;
			}
		}
		
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLubyte TextureCoordinate[] = {
			0, 1,
			0, 0,
			1, 1,
			1, 0
		};
		
		int X = 100;
		int Y = 50;
		int Width = tex.Width;
		int Height = tex.Height;
		
		GLshort Positions[] = {
			X, Y + Height,
			X, Y,
			X + Width, Y + Height,
			X + Width, Y
		};

		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, Positions);
		glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, TextureCoordinate);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		swl_swap();
	}

	quit:
	swl_quit();
	
	return 0;
}
