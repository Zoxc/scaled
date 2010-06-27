#pragma once
#include "layer.hpp"
#include "scene.hpp"

namespace River
{
	Layer::Layer()
	{
		object_lists = new ObjectList[Scene::state_count]();
	}

	Layer::~Layer()
	{
		delete[] object_lists;
	}

	void Layer::render()
	{
		for(size_t l = 0; l < Scene::state_count; l++)
		{
			Scene::state_array[l]->use();

			GLenum error = glGetError();

			if(error)
				assert(0);

			ObjectList &object_list = object_lists[l];

			for(auto i = object_list.begin(); i; i++)
				i().render();
		}
	}
};
