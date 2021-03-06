#include "label.hpp"
#include "../color.hpp"
#include "../scene/fonts/glyph-canvas.hpp"
#include "../scene/fonts/font-size.hpp"
#include "../scene/fonts/glyph.hpp"
#include "../scene/scene.hpp"

namespace River
{	
	Label::Label() : font_size(0)
	{
	}
	
	FontSize *Label::get_font_size()
	{
		if(font_size)
			return font_size;

		return Scene::basic_font.get_size(12);
	}

	void Label::content_size(int &width, int &height)
	{
		width = caption_width;
		height = get_font_size()->line_height;
	}
	
	void Label::set_color(color_t color)
	{
		this->color = color;
	}

	void Label::set_caption(std::string value)
	{
		caption = value;
		caption_width = GlyphCanvas::measure_text(get_font_size(), caption);
	}

	void Label::place(LayerCanvas *layer, int x, int y)
	{
		GlyphCanvas *glyph_canvas = GlyphCanvas::acquire(layer);

		glyph_canvas->render_text(layer, x, y, caption.c_str(), get_font_size(), color);
	}
};
