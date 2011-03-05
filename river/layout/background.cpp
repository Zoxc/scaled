#include "background.hpp"

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
		content->place(layer, x + content->rect.left, y + content->rect.top);
	}

	void Background::layout(int available_width, int available_height)
	{
		Extends &margins = *content->margins;

		content_width = std::max(available_width - (margins.left + margins.right), 0);
		content_height = std::max(available_height - (margins.top + margins.bottom), 0);

		content->layout(content_width, content_height);
		
		content_width = content->rect.width + margins.left + margins.right;
		content_height = content->rect.height + margins.top + margins.bottom;

		Element::layout(available_width, available_height);
		
		background->layout(rect.width, rect.height);
	}
};
