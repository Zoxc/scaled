#include <png.h>
#include "image.hpp"

namespace River
{
	Image::Image(Atlas<GL_RGBA> *atlas) : atlas(atlas), texture(GL_NONE)
	{
	}

	void Image::load_png(std::string filename)
	{
		png_structp png_ptr;
		png_infop info_ptr;
		png_bytepp row_pointers;
		unsigned char* image_data;

		char header[8];

		FILE *file = fopen(filename.c_str(), "rb");
		if(!file)
		{
			printf("File %s could not be opened for reading\n", filename.c_str());
			return;
		}

		fread(header, 1, 8, file);

		if(png_sig_cmp((png_byte*)header, 0, 8))
			printf("File %s is not recognized as a PNG file\n", filename.c_str());

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

		if(!png_ptr)
		{
			printf("png_create_read_struct failed on file: %s\n", filename.c_str());
			return;
		}

		info_ptr = png_create_info_struct(png_ptr);

		if(!info_ptr)
		{
			printf("png_create_info_struct failed on file: %s\n", filename.c_str());
			return;
		}

		if(setjmp(png_jmpbuf(png_ptr)))
		{
			printf("Error during init_io\n");
			return;
		}

		png_init_io(png_ptr, file);
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		width = info_ptr->width;
		height = info_ptr->height;

		// Expand to RGB
		png_set_expand(png_ptr);
		//png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

		unsigned int row_size = png_get_rowbytes(png_ptr, info_ptr);

		image_data = new unsigned char[row_size * height + 4096];

		row_pointers = new png_bytep[row_size];

		for(size_t i = 0; i < height; ++i)
			row_pointers[i] = image_data + i * row_size;

		png_read_image(png_ptr, row_pointers);

		delete[] row_pointers;

		assert(info_ptr->channels == 4 && info_ptr->bit_depth == 8);

		texture = atlas->place(&entry, width, height, image_data);

		delete[] image_data;

		png_destroy_read_struct(&png_ptr, &info_ptr, 0);

		fclose(file);
	}
};
