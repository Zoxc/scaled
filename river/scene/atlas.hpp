#pragma once
#include <algorithm>
#include <stdint.h>
#include "../gles.hpp"
#include "../simple-list.hpp"

namespace River
{
	class AtlasEntry
	{
	public:
		GLfloat x;
		GLfloat y;
		GLfloat x2;
		GLfloat y2;
	};

	template<GLenum type> class Atlas
	{
	public:
		static const size_t width = 256;
		static const size_t height = 128;
		
		class Texture
		{
		private:
			size_t x;
			size_t y;
			size_t line_height;
		public:
			Texture() : x(0), y(0), line_height(0)
			{
				glGenTextures(1, &texture);

				glBindTexture(GL_TEXTURE_2D, texture);

				glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, 0);
		
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			~Texture()
			{
				glDeleteTextures(1, &texture);
			}
			
			GLuint texture;
			SimpleEntry<Texture> entry;

			bool place(AtlasEntry *entry, size_t width, size_t height, uint8_t *raster)
			{
				if(Atlas::width - x < width)
				{
					x = 0;
					y += line_height;
					line_height = 0;
				}
			
				if(Atlas::height - y < height)
					return false;

				glBindTexture(GL_TEXTURE_2D, texture);
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, type, GL_UNSIGNED_BYTE, raster);
			
				entry->x = (GLfloat)x / (GLfloat)Atlas::width;
				entry->y = (GLfloat)y / (GLfloat)Atlas::height;

				entry->x2 = (GLfloat)(x + width) / (GLfloat)Atlas::width;
				entry->y2 = (GLfloat)(y + height) / (GLfloat)Atlas::height;
			
				x += width;
				line_height = std::max(line_height, height);

				return true;
			}
		};
		
	private:
		typedef SimpleList<Texture> TextureList;

		TextureList textures;
		Texture *current_texture;

	public:
		Atlas() : current_texture(new Texture) {}
		
		Texture *place(AtlasEntry *entry, size_t width, size_t height, uint8_t *raster)
		{
			assert(width <= Atlas::width && height <= Atlas::height);

			while(!current_texture->place(entry, width, height, raster))
			{
				textures.append(current_texture);
				current_texture = new Texture;
			}

			return current_texture;
		}
	};
};
