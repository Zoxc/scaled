#include "category.hpp"
#include "../../river/scene/fonts/glyph-canvas.hpp"
#include "../../river/scene/colored-image-canvas.hpp"
#include "../../river/scene/scene.hpp"
#include "../../river/river.hpp"

namespace Launcher
{	
	CategoryWidget::CategoryWidget()
	{
		font_size = River::Scene::basic_font.get_size(12);
	}
	
	void CategoryWidget::content_size(int &width, int &height)
	{
		width = title_width + icon->width + 5;
		height = std::max(font_size->line_height, icon->height);
	}
	
	void CategoryWidget::set_title(std::string value)
	{
		title = value;
		title_width = River::GlyphCanvas::measure_text(font_size, title);
	}

	void CategoryWidget::set_icon(River::Image *icon)
	{
		this->icon = icon;
	}
	
	void CategoryWidget::place(River::LayerCanvas *layer, int x, int y)
	{
		River::ColoredImageCanvas *colored_image_canvas = River::ColoredImageCanvas::acquire(layer);
		River::ColoredImageCanvas *colored_image_canvas_high = River::ColoredImageCanvas::acquire(layer->raise());
		River::GlyphCanvas *glyph_canvas = River::GlyphCanvas::acquire(layer);
		River::GlyphCanvas *glyph_canvas_high = River::GlyphCanvas::acquire(layer->raise());
		River::color_t tint = false ? 0xba9565ff : 0x7f837fff;
		
		colored_image_canvas->render_image(layer, 1 + x, 1 + y + River::center(icon->height, rect.height), icon->width, icon->height, River::color_black, icon);
		glyph_canvas->render_text(layer, 1 + x + 5 + icon->width, 1 + y + River::center(font_size->line_height, rect.height), title.c_str(), font_size, River::color_black);
		
		colored_image_canvas_high->render_image(layer, x, y + River::center(icon->height, rect.height), icon->width, icon->height, tint, icon);
		glyph_canvas_high->render_text(layer, x + 5 + icon->width, y + River::center(font_size->line_height, rect.height), title.c_str(), font_size, tint);
	}
};
