#include "icon.hpp"
#include "../../river/scene/fonts/glyph-canvas.hpp"
#include "../../river/scene/colored-image-canvas.hpp"
#include "../../river/scene/scene.hpp"
#include "../../river/river.hpp"

namespace Launcher
{	
	IconWidget::IconWidget()
	{
		font_size = River::Scene::basic_font.get_size(11);
	}
	
	void IconWidget::content_size(int &width, int &height)
	{
		width = 70;
		height = 70;
	}
	
	void IconWidget::set_title(std::string value)
	{
		title = value;
		title_width = River::GlyphCanvas::measure_text(font_size, title);
	}

	void IconWidget::set_icon(River::Image *icon)
	{
		this->icon = icon;
	}
	
	void IconWidget::place(River::LayerCanvas *layer, int x, int y)
	{
		River::ColoredImageCanvas *colored_image_canvas = River::ColoredImageCanvas::acquire(layer);
		River::ColoredImageCanvas *colored_image_canvas_high = River::ColoredImageCanvas::acquire(layer->raise());
		River::GlyphCanvas *glyph_canvas = River::GlyphCanvas::acquire(layer);
		River::GlyphCanvas *glyph_canvas_high = River::GlyphCanvas::acquire(layer->raise());

		River::color_t tint = false ? 0xba9565ff : 0x7f837fff;
		
		colored_image_canvas->render_image(layer, x + River::center(icon->width, rect.width) + 1, y + 1, icon->width, icon->height, River::color_black, icon);
		colored_image_canvas_high->render_image(layer, x + River::center(icon->width, rect.width), y, icon->width, icon->height, tint, icon);

		x += River::center(title_width, rect.width);
		y += 35;
		glyph_canvas_high->render_text(layer, x++, y++, title.c_str(), font_size, tint);
		glyph_canvas->render_text(layer, x, y, title.c_str(), font_size, River::color_black);
	}
};
