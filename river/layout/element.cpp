#pragma once
#include "element.hpp"

namespace River
{
	Extends Element::no_margins(0, 0, 0, 0);

	void Element::layout(int available_width, int available_height)
	{
		int def_width, def_height;

		content_size(def_width, def_height);

		switch(width)
		{
		case Flags::Auto:
			rect.width = def_width;
			break;

		case Flags::Extend:
			rect.width = available_width;
			break;

		default:
			rect.width = width;
		}
		
		switch(height)
		{
		case Flags::Auto:
			rect.height = def_height;
			break;

		case Flags::Extend:
			rect.height = available_height;
			break;

		default:
			rect.height = height;
		}
	}

	void Element::content_size(int &width, int &height)
	{
		width = 10;
		height = 10;
	}
};
