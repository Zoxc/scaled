#pragma once
#include <algorithm>
#include "../list.hpp"
#include "../simple-list.hpp"
#include "../scene/layer.hpp"
#include "extends.hpp"

namespace River
{
	class Element
	{
	public:
		Element() : margins(&no_margins), flags(0), weight(1), width(Flags::Auto), height(Flags::Auto) {}
		
		static Extends no_margins;

		int calculate_size(int available_width, int available_height, int def_width, int def_height);
		
		Entry<Element> children_entry;
		SimpleEntry<Element> extend_entry;

		class Flags
		{
		public:
			static const int Auto = -1;
			static const int Extend = -2;

			static const size_t OwnsMargins = 1;
		};

		struct Rect
		{
			int left;
			int top;
			int width;
			int height;
		};
		
		size_t flags;
		Extends *margins;

		/*
		 * Used for layout input. Can hold Auto and Extend.
		 */ 
		int width;
		int height;
		
		size_t weight;

		Rect rect;

		int min_width;
		int min_height;

		/*
		 * layout() must store width and height in rect.
		 * available_width and available_height should have room for min_width and min_height.
		 */
		virtual void layout(int available_width, int available_height);
		
		/*
		 * place() expects the elements position to be stored in rect.
		 */
		virtual void place(Layer *layer, int x, int y) = 0;

		/*
		 * place() expects the elements position to be stored in rect.
		 */
		virtual void content_size(int &width, int &height);
	};

};
