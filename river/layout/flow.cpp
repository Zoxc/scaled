#include "flow.hpp"

namespace River
{
	void Flow::layout(int available_width, int available_height)
	{
		Element *element = children.first;
		int height = available_height;
		int margin_top = padding->top;
		int max_width = 0;
		int top = 0;
		
		while(element)
		{
			Element *line_start = element;
			ExtendList extends;
			int line_height = margin_top;
			int line_margin_bottom = line_height;
			int margin_left = padding->left;
			int width = available_width;
			Element *line_end = 0;

			/*
			 * First pass, just find the end of the line.
			 */

			while(element)
			{
				Extends margins(margin_left, margin_top, padding->right, padding->bottom);
				margins.union_with(element->margins);
				
				int element_width = std::max(width - (margins.left + margins.right), 0);
				
				/*
				 * Delay layout of extended elements until we know the actual size of it.
				 */
				if(element->width == Flags::Extend || element->height == Flags::Extend)
				{
					/*
					 * Use the minimum size as a placeholder in calculations.
					 */
					element->rect.width = element->min_width;
					element->rect.height = element->min_height;
				}
				else
				{
					int element_height = std::max(height - (margins.top + margins.bottom), 0);

					element->layout(element_width, element_height);
				}

				/*
				 * Make sure we can fit the element on this line.
				 * Only give it a new line if something reduced the space on this one.
				 */
				if(element->rect.width > element_width && width != available_width)
				{
					line_end = element->children_entry.prev;
					break;
				}
				
				/*
				 * Add horizontally extended elements to the list once we are sure they're on this line.
				 */
				if(element->width == Flags::Extend)
					extends.append(element);

				width -= margins.left + element->rect.width;
				margin_left = element->margins->right;

				int element_span = element->rect.height + margins.top;

				line_height = std::max(line_height, element_span);
				line_margin_bottom = std::max(line_margin_bottom, element_span + element->margins->bottom);
				
				line_end = element;

				element = element->children_entry.next;
			}
			
			/*
			 * Include the right margin of the last element in the width calculation.
			 */
			if(line_end)
				width -= std::max(padding->right, line_end->margins->right);
			else
				width -= padding->right;

			max_width = std::max(max_width, available_width - width);

			/* 
			 * Second pass, extend elements horizontally.
			 */
			if(extends.first)
			{
				/* 
				 * Calculate the total element weight.
				 */
				size_t weight = 0;

				for(ExtendList::Iterator i = extends.begin(); i != extends.end(); ++i)
					weight += i().weight;

				/*
				 * Layout elements.
				 */
				for(ExtendList::Iterator i = extends.begin(); i != extends.end(); ++i)
				{
					i().rect.width += width * i().weight / weight;

					/*
					 * If the element is not height extended, layout it now.
					 */
					if(i().height != Flags::Extend)
					{
						i().layout(i().rect.width, i().rect.height);
						
						/*
						 * Update line height and margin if needed.
						 */
						int element_span = i().rect.height + std::max(margin_top, i().margins->top);

						line_height = std::max(line_height, element_span);
						line_margin_bottom = std::max(line_margin_bottom, element_span + i().margins->bottom);
					}
				}
			}

			/*
			 * Height is calculated. Adjust margins.
			 */
			line_margin_bottom -= line_height;

			/*
			 * Third pass, extend elements vertically and position all the elements.
			 */

			Element *next_line = element;
			element = line_start;
			margin_left = padding->left;
			int left = 0;

			while(element)
			{
				Extends margins(margin_left, margin_top, padding->right, padding->bottom);
				margins.union_with(element->margins);
				
				/*
				 * Extend element vertically, if needed.
				 */
				if(element->height == Flags::Extend)
				{
					int height = line_height - margins.top;

					height += line_margin_bottom - element->margins->bottom;

					element->rect.height = height;

					element->layout(element->rect.width, element->rect.height);
				}

				left += margins.left;

				element->rect.left = left;
				element->rect.top = top + margins.top;

				left += element->rect.width;

				margin_left = element->margins->right;

				element = element->children_entry.next;
			}

			top += line_height;
			margin_top = line_margin_bottom;
			element = next_line;
		}
		
		rect.width = max_width;
		rect.height = top + std::max(margin_top, padding->bottom);
	}
};
