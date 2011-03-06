#pragma once
#include <string>
#include "../../river/layout/element.hpp"
#include "../../river/scene/fonts/font-size.hpp"
#include "../../river/image.hpp"

namespace Launcher
{
	class CategoryWidget:
		public River::Element
	{
	private:
		River::FontSize *font_size;
		River::Image *icon;
		int title_width;
		std::string title;

	protected:
		void content_size(int &width, int &height);

	public:
		CategoryWidget();
		
		void set_icon(River::Image *icon);
		void set_title(std::string value);

		void place(River::LayerCanvas *layer, int x, int y);
	};

};
