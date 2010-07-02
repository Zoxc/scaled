#pragma once
#include "window.hpp"

namespace River
{
	void Window::render()
	{
		for(auto i = layers.begin(); i; i++)
			i().render();
	}
};
