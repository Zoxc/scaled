#pragma once
#include "../list.hpp"
#include "object-list.hpp"
#include "object-hash.hpp"
#include "gradient-object.hpp"
#include "fonts/glyph-object.hpp"

namespace River
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		Entry<Layer> window_entry;
		
		ObjectList<GradientObject> gradient_object_list;
		ObjectHash<GlyphCache *, GlyphObject> glyph_object_hash;

		void render();
	};
};
