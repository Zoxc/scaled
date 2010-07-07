#include <iostream>
#include <png.h>
#include <stdarg.h>
#include "OpenGL.Texture.hpp"

OpenGL::Texture::Texture() : Allocated(false)
{
}

OpenGL::Texture::~Texture()
{
    if(Allocated)
        glDeleteTextures(1, &Handle);
}

void OpenGL::Texture::Load(const char* FileName)
{
    png_structp PngPtr;
    png_infop InfoPtr;
    png_bytepp RowPointers;
    unsigned char* ImageData;

	char Header[8];

	if(!Allocated)
        glGenTextures(1, &Handle);

	FILE *File = fopen(FileName, "rb");
	if(!File)
	{
		printf("File %s could not be opened for reading\n", FileName);
		return;
	}

	fread(Header, 1, 8, File);

	if(png_sig_cmp((png_byte*)Header, 0, 8))
		printf("File %s is not recognized as a PNG file\n", FileName);

	PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	if(!PngPtr)
	{
		printf("png_create_read_struct failed on file: %s\n", FileName);
		return;
	}

	InfoPtr = png_create_info_struct(PngPtr);

	if(!InfoPtr)
	{
		printf("png_create_info_struct failed on file: %s\n", FileName);
		return;
	}

	if(setjmp(png_jmpbuf(PngPtr)))
	{
		printf("Error during init_io\n");
		return;
	}

	png_init_io(PngPtr, File);
	png_set_sig_bytes(PngPtr, 8);

    png_read_info(PngPtr, InfoPtr);
	
	png_uint_32 width, height;
	int bpp, color_type;

	png_get_IHDR(PngPtr, InfoPtr, &width, &height, &bpp, &color_type, NULL, NULL, NULL);

	// Convert stuff to appropriate formats!
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(PngPtr);
	
	if (color_type == PNG_COLOR_TYPE_GRAY && bpp < 8)
		png_set_gray_1_2_4_to_8(PngPtr);

	if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(PngPtr);

	if (bpp == 16)
		png_set_strip_16(PngPtr);

    Width = width;
    Height = height;
	
	GLenum format;
	
	int channels = png_get_channels(PngPtr, InfoPtr);

	switch (channels)
	{
	case 1: 
		format = GL_ALPHA;
		break;
		
	case 3:	
		format = GL_RGB;
		break;
		
	case 4: 
		format = GL_RGBA;
		break;
		
	default:
		throw "Invalid channel count.";
	}

    unsigned int RowSize = Width * channels;

    ImageData = new unsigned char[RowSize * Height + 4096];

    RowPointers = new png_bytep[RowSize];

    for (int i = 0;  i < Height; i++)
        RowPointers[i] = ImageData + i * RowSize;

    png_read_image(PngPtr, RowPointers);
	
	png_read_end(PngPtr, InfoPtr);

    delete[] RowPointers;

    glBindTexture(GL_TEXTURE_2D, Handle);
	
    glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, ImageData);
	
    delete[] ImageData;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    png_destroy_read_struct(&PngPtr, &InfoPtr, 0);

    fclose(File);
}
