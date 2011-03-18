#pragma once
#include "container.hpp"

namespace River
{
	class Background:
		public Container
	{
	private:
		int content_width;
		int content_height;
		Element *background;
		Element *content;
	protected:
		void content_size(int &width, int &height);
	public:
		Background();

		void set_background(Element *background);
		void set_content(Element *content);
		void layout(int available_width, int available_height);
		void place(LayerCanvas *layer, int x, int y);
	};
};
