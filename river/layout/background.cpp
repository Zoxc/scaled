#include "background.hpp"
#include "../scene/layer-canvas.hpp"

namespace River
{
	Background::Background() : background(0), content(0)
	{
	}
	
	void Background::set_background(Element *background)
	{
		background->width = Element::Flags::Extend;
		background->height = Element::Flags::Extend;
		this->background = background;
	}

	void Background::set_content(Element *content)
	{
		this->content = content;
	}

	void Background::content_size(int &width, int &height)
	{
		width = content_width;
		height = content_height;
	}
	
	void Background::place(LayerCanvas *layer, int x, int y)
	{
		background->place(layer, x, y);
		content->place(layer->raise(), x + content->rect.left, y + content->rect.top);
	}

	void Background::layout(int available_width, int available_height)
	{
		Extends &margins = *content->margins;

		content_width = std::max(available_width - (margins.left + margins.right), 0);
		content_height = std::max(available_height - (margins.top + margins.bottom), 0);

		content->layout(content_width, content_height);

		content->rect.left = margins.left;
		content->rect.top = margins.top;
		
		content_width = content->rect.width + margins.left + margins.right;
		content_height = content->rect.height + margins.top + margins.bottom;

		Element::layout(available_width, available_height);
		
		background->layout(rect.width, rect.height);
	}
	
	bool Background::mouse_outside(int x, int y)
	{
		if(Element::mouse_outside(x, y))
		{
			flags &= ~Flags::MouseOver;
			
			x -= content->rect.left;
			y -= content->rect.top;

			content->mouse_outside(x, y);

			return true;
		}
		else
			return false;
	}

	void Background::mouse_event(MouseEvent event, int x, int y)
	{
		int content_x = x - content->rect.left;
		int content_y = y - content->rect.top;

		if(content->inside(content_x, content_y))
		{
			mouse_inside(x, y);

			content->mouse_event(event, content_x, content_y);
		}
		else
		{
			content->mouse_outside(content_x, content_y);

			Element::mouse_event(event, x, y);
		}
	}
};
