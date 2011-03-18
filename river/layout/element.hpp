#pragma once
#include <algorithm>
#include "../river.hpp"
#include "../list.hpp"
#include "../simple-list.hpp"
#include "extends.hpp"

namespace River
{
	class LayerCanvas;

	class Element
	{
	protected:
		/*
		 * place() expects the elements position to be stored in rect.
		 */
		virtual void content_size(int &width, int &height);

		/*
		 * Convenience function for layouting.
		 */
		void simple_layout(int available_width, int available_height, int content_width, int content_height);

		template<Orientation orientation> void generic_simple_layout(int available_primary, int available_secondary, int content_primary, int content_secondary)
		{
			if(orientation == Horizontal)
				simple_layout(available_primary, available_secondary, content_primary, content_secondary);
			else
				simple_layout(available_secondary, available_primary, content_secondary, content_primary);
		}

	public:
		class Flags
		{
		public:
			static const int Auto = -1;
			static const int Extend = -2;
			
			static const size_t MouseOver = 1;
			static const size_t Visible = 2;
		};
		
		Element() : flags(Flags::Visible), margins(&no_margins), width(Flags::Auto), height(Flags::Auto), weight(1) {}
		
		static Extends no_margins;

		int calculate_size(int available_width, int available_height, int def_width, int def_height);
		
		Entry<Element> children_entry;
		SimpleEntry<Element> extend_entry;

		struct Rect
		{
			int left;
			int top;
			int width;
			int height;

			struct Accessors
			{
				int Rect::*primary_offset;
				int Rect::*primary_length;
				int Rect::*secondary_offset;
				int Rect::*secondary_length;
			};
		
			static const Accessors accessors[2];
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
		
		struct Accessors
		{
			int Element::*primary_length;
			int Element::*primary_min_length;
			int Element::*secondary_length;
			int Element::*secondary_min_length;
		};
		
		static const Accessors accessors[2];
		
		template<Orientation orientation> void generic_layout(int available_primary, int available_secondary)
		{
			if(orientation == Horizontal)
				layout(available_primary, available_secondary);
			else
				layout(available_secondary, available_primary);
		}

		/*
		 * layout() must store width and height in rect.
		 * available_width and available_height should have room for min_width and min_height.
		 */
		virtual void layout(int available_width, int available_height);
		
		/*
		 * place() expects the elements position to be stored in rect.
		 */
		virtual void place(LayerCanvas *layer, int x, int y) = 0;

		
		bool inside(int x, int y);
		bool mouse_inside(int x, int y);
		virtual bool mouse_outside(int x, int y);
		virtual void mouse_event(MouseEvent event, int x, int y);
	};

};
