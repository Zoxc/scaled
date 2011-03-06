#pragma once
#include <string>
#include "../layout/element.hpp"
#include "../color.hpp"

namespace River
{
	class FontSize;

	class Label:
		public Element
	{
	private:
		int caption_width;
		std::string caption;
		color_t color;
		FontSize *font_size;
		FontSize *get_font_size();

	protected:
		void content_size(int &width, int &height);

	public:
		Label();
		
		void set_caption(std::string value);
		void set_color(color_t color);

		void place(LayerCanvas *layer, int x, int y);
	};

};
