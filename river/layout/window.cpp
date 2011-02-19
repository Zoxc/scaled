#include "window.hpp"

namespace River
{
	void Window::render()
	{
		for(LayerList::Iterator i = layers.begin(); i != layers.end(); ++i)
			i().render();
	}
};
