#pragma once
#include <vector>
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

	private:
		std::vector<Content *> list;

	public:
		Layer();
		~Layer();
		
		Entry<Layer> window_entry;

		void append(Content *content);
		void render();
	};
};
