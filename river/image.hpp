#pragma once
#include <string>
#include "gles.hpp"
#include "scene/atlas.hpp"

namespace River
{
	class ImageException
	{
		
	};

	class Image
	{
	private:
		Atlas<GL_RGBA> *atlas;
	public:
		Atlas<GL_RGBA>::Texture *texture;
		AtlasEntry entry;

		size_t width;
		size_t height;

		Image(Atlas<GL_RGBA> *atlas);

		void load_png(std::string filename);
	};
};
