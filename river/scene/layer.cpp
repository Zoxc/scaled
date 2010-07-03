#include "layer.hpp"
#include "scene.hpp"

namespace River
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::render()
	{
		Scene::gradient_state.use();
		gradient_object_list.render();

		GLenum error = glGetError();

		if(error)
			assert(0);
	}
};
