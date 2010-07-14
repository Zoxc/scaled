#include <stdio.h>
#include <math.h>
#include <swl.h>

#include "gles.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;

const int width = 1200;
const int height = 480;

GLuint program;

GLuint TextureUniform;

void draw_quad(int x, int y, int width, int height)
{	
	GLubyte TextureCoordinate[] = {
		0, 1,
		0, 0,
		1, 1,
		1, 0
	};
	
	GLshort Positions[] = {
		x, y + height,
		x, y,
		x + width, y + height,
		x + width, y
	};
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, Positions);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, TextureCoordinate);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	
}

bool do_correct = true;
bool invert = true;
double gamma = 1.4;
double bold_eff = 1;

double to_linear(double value)
{
	if(!do_correct)
		return value;
	
	double result = pow(value, bold_eff);
	
	result = pow(result, invert ? 1.0 / gamma : gamma);
	
	return result;
};

double to_srgb(double value)
{
	if(!do_correct)
		return value;
	
	double result = pow(invert ? 1 - value : value, 1.0 / gamma);
	
	return invert ? 1 - result : result;
};

double get_component(FT_Face face, int x, int y)
{
	if(x < 0 || x >= face->glyph->bitmap.width)
		return 0;
		
	unsigned char result;
	
	if(face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
	{
		unsigned char *row = (unsigned char *)face->glyph->bitmap.buffer + face->glyph->bitmap.pitch * y;
		int byte = x / 8;
		int bit = 7 - (x % 8);
		result = ((row[byte] >> bit) & 1) * 255;
	}
	else
		result = ((unsigned char *)face->glyph->bitmap.buffer)[face->glyph->bitmap.pitch * y + x];
	
	return to_linear((double)result / 255.0);
};

unsigned char mix_component(FT_Face face, int x, int y)
{
	double old_gamma = gamma;
	gamma = 1.0;
	double result =
		get_component(face, x - 2, y) * 1 +
		get_component(face, x - 1, y) * 3 +
		get_component(face, x + 0, y) * 4 +
		get_component(face, x + 1, y) * 3 +
		get_component(face, x + 2, y) * 1;
	
	unsigned char result2 = lround(to_srgb(result / 12.0) * 255.0);
	gamma = old_gamma;
	return result2;
};

unsigned char correct(unsigned char c)
{
	if(!do_correct)
		return c;
	
	return lround(to_linear(c / 255.0) * 255.0);
};

GLuint texture_from_rgb2(FT_Face face, int *x, int *mx)
{
	GLuint tex;
	
	*x = 5; *mx = 1;
	
	glGenTextures(1, &tex);
	
    glBindTexture(GL_TEXTURE_2D, tex);
	
	int width = lround(face->glyph->bitmap.width) + 10;
	int height = face->glyph->bitmap.rows;
	
	int data_size = width * height * 4;
	
	unsigned int* PixelData = (unsigned int *)malloc(data_size);

	unsigned int* Pixel = PixelData;
	unsigned int* PixelEnd = (unsigned int *)((unsigned char *)PixelData + data_size);

	int y = 0;
	
	while(Pixel != PixelEnd)
	{
		for(int lx = 0; lx < width; lx++)
		{
			((unsigned char*)Pixel)[0] = correct(mix_component(face, lx * 3 - *x - 1, y));
			((unsigned char*)Pixel)[1] = correct(mix_component(face, lx * 3 - *x, y));
			((unsigned char*)Pixel)[2] = correct(mix_component(face, lx * 3 - *x + 1, y));
			((unsigned char*)Pixel)[3] = 128;
			Pixel++;
		}
		y++;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelData);

	free(PixelData);
	
	return tex;
}

GLuint texture_from_rgb(FT_Face face, int *x, int *mx)
{
	GLuint tex;
	
	*x = 5; *mx = 3;
	
	glGenTextures(1, &tex);
	
    glBindTexture(GL_TEXTURE_2D, tex);
	
	int width = lround(face->glyph->bitmap.width / 3.0) + 10;
	int height = face->glyph->bitmap.rows;
	
	int data_size = width * height * 4;
	
	unsigned int* PixelData = (unsigned int *)malloc(data_size);

	unsigned int* Pixel = PixelData;
	unsigned int* PixelEnd = (unsigned int *)((unsigned char *)PixelData + data_size);

	int y = 0;
	
	while(Pixel != PixelEnd)
	{
		for(int lx = 0; lx < width; lx++)
		{
			((unsigned char*)Pixel)[0] = correct(mix_component(face, lx * 3 - *x - 1, y));
			((unsigned char*)Pixel)[1] = correct(mix_component(face, lx * 3 - *x, y));
			((unsigned char*)Pixel)[2] = correct(mix_component(face, lx * 3 - *x + 1, y));
			((unsigned char*)Pixel)[3] = 128;
			Pixel++;
		}
		y++;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelData);

	free(PixelData);
	
	return tex;
}

GLuint texture_from_mono(FT_Face face, int *x, int *mx)
{
	GLuint tex;
	
	*x = 0; *mx = 1;
	
	glGenTextures(1, &tex);
	
    glBindTexture(GL_TEXTURE_2D, tex);
	
	int width = face->glyph->bitmap.width;
	int height = face->glyph->bitmap.rows;
	
	int data_size = width * height * 4;
	
	unsigned int* PixelData = (unsigned int *)malloc(data_size);

	unsigned int* Pixel = PixelData;
	unsigned int* PixelEnd = (unsigned int *)((unsigned char *)PixelData + data_size);

	unsigned char *row = face->glyph->bitmap.buffer;

	while(Pixel != PixelEnd)
	{
		for(int x = 0; x < width; x++)
		{
			int byte = x / 8;
			int bit = 7 - (x % 8);
			unsigned char value = ((row[byte] >> bit) & 1) * 255;
			
			*Pixel = value;
			((unsigned char*)Pixel)[1] = value;
			((unsigned char*)Pixel)[2] = value;
			((unsigned char*)Pixel)[3] = 128;
			Pixel++;
		}
		
		row += face->glyph->bitmap.pitch;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelData);

	free(PixelData);
	
	return tex;
}

GLuint texture_from_aa(FT_Face face, int *x, int *mx)
{
	GLuint tex;
	*x = 0; *mx = 1;
	
	glGenTextures(1, &tex);
	
    glBindTexture(GL_TEXTURE_2D, tex);
	
	int width = face->glyph->bitmap.width;
	int height = face->glyph->bitmap.rows;
	
	int data_size = width * height * 4;
	
	unsigned int* PixelData = (unsigned int *)malloc(data_size);

	unsigned int* Pixel = PixelData;
	unsigned int* PixelEnd = (unsigned int *)((unsigned char *)PixelData + data_size);

	unsigned char* Buffer = face->glyph->bitmap.buffer;

	while(Pixel != PixelEnd)
	{
		*Pixel = correct(*Buffer);
		((unsigned char*)Pixel)[1] = correct(*Buffer);
		((unsigned char*)Pixel)[2] = correct(*Buffer);
		((unsigned char*)Pixel)[3] = 128;
		Pixel++;
		Buffer++;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelData);

	free(PixelData);
	
	return tex;
}

void render_char_light(FT_Face face, const char c)
{
	if(FT_Load_Char(face, c, FT_LOAD_TARGET_LIGHT))
		printf("FT_Load_Glyph failed");
	
	if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LIGHT))
		printf("FT_Render_Glyph failed");
}

void render_char_full(FT_Face face, const char c)
{
	if(FT_Load_Char(face, c, FT_LOAD_FORCE_AUTOHINT))
		printf("FT_Load_Glyph failed");
	
	if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
		printf("FT_Render_Glyph failed");
}

void render_char_lcd(FT_Face face, const char c)
{
	if(FT_Load_Char(face, c, FT_LOAD_TARGET_LCD))
		printf("FT_Load_Glyph failed");
	
	if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD))
		printf("FT_Render_Glyph failed");
}

void render_char_nohint(FT_Face face, const char c)
{
	if(FT_Load_Char(face, c, FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT))
		printf("FT_Load_Glyph failed");
	
	if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
		printf("FT_Render_Glyph failed");
}

void render_char_mono(FT_Face face, const char c)
{
	if(FT_Load_Char(face, c, FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
		printf("FT_Load_Glyph failed");
	
	if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO))
		printf("FT_Render_Glyph failed");
}

typedef GLuint (*texture_char_t)(FT_Face face, int *x, int *mx);
typedef void (*render_char_t)(FT_Face face, const char c);

void draw_char(FT_Face face, int *x, int y, const char c, render_char_t render_char, texture_char_t texture_char)
{
	render_char(face, c);
	int ox, mx;
	
	if(face->glyph->bitmap.width * face->glyph->bitmap.rows > 0)
	{
		GLuint tex = texture_char(face, &ox, &mx);
		
		draw_quad(*x + lround((double)face->glyph->bitmap_left / (double)mx) - ox, y - face->glyph->bitmap_top, lround((double)face->glyph->bitmap.width / (double)mx) + ox * 2, face->glyph->bitmap.rows);
		
		glDeleteTextures(1, &tex);
	}
	
	*x += lround((double)face->glyph->advance.x / 64.0 / (double)mx);
}

void draw_text(FT_Face face, int *x, int y, const char *text, render_char_t render_char, texture_char_t texture_char)
{
	const char *c = text;
	
	while(*c)
	{
		draw_char(face, x, y, *c, render_char, texture_char);
		c++;
	}
	
	*x += 10;
}

void draw_texts(FT_Face face, int *x, int y, const char *text)
{
	if(FT_Set_Char_Size(face, 0, 8 * 64, 96, 96))
		printf("FT_Set_Char_Size failed.");
	
	draw_text(face, x, y, text, render_char_mono, texture_from_mono);
	draw_text(face, x, y, text, render_char_full, texture_from_aa);
	draw_text(face, x, y, text, render_char_light, texture_from_aa);
	draw_text(face, x, y, text, render_char_nohint, texture_from_aa);
	draw_text(face, x, y, text, render_char_lcd, texture_from_rgb2);
	
	if(FT_Set_Char_Size(face, 0, 8 * 64, 96 * 3, 96))
		printf("FT_Set_Char_Size failed.");
	
	draw_text(face, x, y, text, render_char_nohint, texture_from_rgb);
	draw_text(face, x, y, text, render_char_full, texture_from_rgb);
	draw_text(face, x, y, text, render_char_light, texture_from_rgb);
	draw_text(face, x, y, text, render_char_mono, texture_from_rgb);
}

void draw_font(int *y, const char *filename)
{
	FT_Face face;
	
	if(FT_New_Face(library, filename, 0, &face))
		printf("FT_New_Face failed.");
	
	int x = 10;
	const char *text = "wreck-openpand~";
	do_correct = false;
	
	draw_texts(face, &x, *y, text);
	
	*y += 25;
	x = 10;
	
	do_correct = true;
	draw_texts(face, &x, *y, text);
	
	*y += 25;
	
	FT_Done_Face(face);
}

int main(void)
{
	if(FT_Init_FreeType(&library))
	{
		printf("FreeType setup failed");
		return -1;
	}
	
	enum swl_result result = swl_init("blending", width, height, true);

	if(result != SWLR_OK)
	{
		printf("Unable to setup window... %d", result);
		return -1;
	}
	
	#ifdef SWL_OPENGL_ES
		#define SHADER_HEADER "precision highp float;"
	#else
		#define SHADER_HEADER sfs
	#endif
	
	const char* fragment_shader =
		SHADER_HEADER
		"varying vec2 VCord;\
		uniform sampler2D Texture;\
		void main (void)\
		{\
			gl_FragColor = texture2D(Texture, VCord); \
		}";

	const char* vertex_shader =
		"precision highp float;"
		"attribute vec2 APoint;\
		attribute vec2 ACord;\
		varying vec2 VCord;\
		void main(void)\
		{\
			gl_Position.x = APoint.x / 600.0 - 1.0;\
			gl_Position.y = -(APoint.y / 240.0 - 1.0);\
			gl_Position.z = 0.0;\
			gl_Position.w = 1.0;\
			VCord = ACord;\
		}";

	GLfloat color[4] = {1, 0, 0, 1};
	
	program = glCreateProgram();
	
	gluCompileShader(program, GL_FRAGMENT_SHADER, fragment_shader);
	gluCompileShader(program, GL_VERTEX_SHADER, vertex_shader);
	
	glBindAttribLocation(program, 0, "APoint");
	glBindAttribLocation(program, 1, "ACord");

	gluLinkProgram(program);
	
	glUseProgram(program);
	
	TextureUniform = glGetUniformLocation(program, "Texture");

	glUniform1i(TextureUniform, 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBlendColor(color[0], color[1], color[2], color[3]);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glActiveTexture(GL_TEXTURE0);
	
	GLenum error = glGetError();
	
	if(error != GL_NO_ERROR)
	{
		printf("OpenGL failed with error 0x%x.\n", error);
		return 0;
	}
	
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
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glScissor(0, 240, 1200, 240);
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glScissor(0, 0, 1200, 480);
		
		glBlendColor(0.0, 0.0, 0.0, 1.0);
		invert = false;
		
		int y = 25;
		draw_font(&y, "fonts\\DroidSans.ttf");
		draw_font(&y, "fonts\\cour.ttf");
		draw_font(&y, "fonts\\DejaVuSansMono.ttf");

		glBlendColor(1.0, 1.0, 1.0, 1.0);
		invert = true;
		
		y = 25 + 240;
		draw_font(&y, "fonts\\DroidSans.ttf");
		draw_font(&y, "fonts\\cour.ttf");
		draw_font(&y, "fonts\\DejaVuSansMono.ttf");
		
		swl_swap();
		
		
		
		error = glGetError();
		
		if(error != GL_NO_ERROR)
		{
			printf("OpenGL failed in scene with error 0x%x.\n", error);
			return 0;
		}
	}

	quit:
	swl_quit();
	
	return 0;
}
