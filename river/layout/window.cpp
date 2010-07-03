#include "window.hpp"

namespace River
{
	void Window::render()
	{
		for(LayerList::Iterator i = layers.begin(); i; i++)
			i().render();
	}
};
