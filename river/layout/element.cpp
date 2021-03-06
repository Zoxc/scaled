#include "element.hpp"

namespace River
{
	const Element::Rect::Accessors Element::Rect::accessors[2] = {
		{&Element::Rect::left, &Element::Rect::width, &Element::Rect::top, &Element::Rect::height},
		{&Element::Rect::top, &Element::Rect::height, &Element::Rect::left, &Element::Rect::width}
	};
	
	const Element::Accessors Element::accessors[2] = {
		{&Element::width, &Element::min_width, &Element::height, &Element::min_height},
		{&Element::height, &Element::min_height, &Element::width, &Element::min_width}
	};
	
	Extends Element::no_margins(0, 0, 0, 0);
	
	void Element::simple_layout(int available_width, int available_height, int content_width, int content_height)
	{
		switch(width)
		{
		case Flags::Auto:
			rect.width = content_width;
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
			rect.height = content_height;
			break;

		case Flags::Extend:
			rect.height = available_height;
			break;

		default:
			rect.height = height;
		}
	}

	void Element::layout(int available_width, int available_height)
	{
		int content_width, content_height;

		content_size(content_width, content_height);
		
		simple_layout(available_width, available_height, content_width, content_height);
	}

	void Element::content_size(int &width, int &height)
	{
		width = 10;
		height = 10;
	}
	
	bool Element::mouse_outside(int x, int y)
	{
		if(flags & Flags::MouseOver)
		{
			flags &= ~Flags::MouseOver;
			
			mouse_event(MouseEventLeave, x, y);

			return true;
		}
		else
			return false;
	}
	
	bool Element::mouse_inside(int x, int y)
	{
		if(!(flags & Flags::MouseOver))
		{
			flags |= Flags::MouseOver;

			mouse_event(MouseEventEnter, x, y);

			return true;
		}
		else
			return false;
	}

	bool Element::inside(int x, int y)
	{
		// Cast to unsigned for faster testing
		return (size_t)x <= (size_t)rect.width && (size_t)y <= (size_t)rect.height;
	}

	void Element::mouse_event(MouseEvent event, int x, int y)
	{
		mouse_inside(x, y);

		if(event == MouseEventDown)
			printf("Down on %x at %d, %d\n", this, x, y);
	}
};
