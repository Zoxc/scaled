#include "layer.hpp"
#include "scene.hpp"

namespace River
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		for(std::vector<Content *>::iterator i = list.begin(); i != list.end(); ++i)
			delete *i;
	}

	void Layer::render()
	{
		for(std::vector<Content *>::iterator i = list.begin(); i != list.end(); ++i)
			(*i)->render();

		Scene::raise_errors();
	}

	void Layer::append(Content *content)
	{
		list.push_back(content);
	}
};
