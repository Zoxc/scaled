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
		bool Allocated;
		size_t width;
		size_t height;
		Atlas<GL_RGBA> *atlas;
		AtlasEntry entry;
	public:
		Atlas<GL_RGBA>::Texture *texture;

		Image(Atlas<GL_RGBA> *atlas);

		void load_png(std::string filename);
	};
};
