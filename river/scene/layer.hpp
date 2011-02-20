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
		class Content
		{
			public:
				virtual void render() = 0;

				virtual ~Content()
				{
				}
		};

		Layer();
		~Layer();

		Entry<Layer> window_entry;
		
		void render();
	};
};
