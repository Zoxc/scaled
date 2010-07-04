#include "flow.hpp"

namespace River
{
	void Flow::layout(int available_width, int available_height)
	{
		Element *element = children.first;
		int height = available_height;
		int top = 0;
		int margin_top = padding->top;
		
		rect.width = available_width;
		rect.height = available_height;

		while(element)
		{
			Line line;
			line.start = element;
			ExtendList extends;
			int margin_left = padding->left;
			int width = available_width;
			line.height = margin_top;
			Element *line_end = 0;

			/*
			 * First pass, just find the end of the line.
			 */

			while(element)
			{
				Extends margins(margin_left, margin_top, padding->right, padding->bottom, element->margins);
				
				int element_width = std::max(width - (margins.left + margins.right), 0);
				int element_height = std::max(height - (margins.top + margins.bottom), 0);
				
				/*
				 * Delay layout of extended elements until we know the actual size of it.
				 */
				if(element->width == Flags::Extend)
				{
					extends.append(element);
					
					/*
					 * Use the minimum size as a placeholder in calculations.
					 */
					element->rect.width = element->min_width;
					element->rect.height = element->min_height;
				}
				else
					element->layout(element_width, element_height);
				
				/*
				 * Make sure we can fit the element on this line.
				 * Only give it a new line if something reduced the space on this one.
				 */
				if(element->rect.width > element_width && width != available_width)
				{
					line_end = element->children_entry.prev;
					break;
				}

				width -= margins.left + element->rect.width;
				margin_left = element->margins->right;
				line.height = std::max(line.height, element->rect.height + margins.top);
				
				line_end = element;

				element = element->children_entry.next;
			}
			
			/*
			 * Include the right margin of the last element in the width calculation.
			 */
			if(line_end)
				width -= std::max(padding->right, line_end->margins->right);

			/* 
			 * Second pass, extend elements.
			 */
			if(extends.first)
			{
				/* 
				 * Calculate the total element weight.
				 */
				size_t weight = 0;

				for(ExtendList::Iterator i = extends.begin(); i; i++)
					weight += i().weight;

				/*
				 * Layout elements.
				 */
				for(ExtendList::Iterator i = extends.begin(); i; i++)
				{
					i().rect.width += width * i().weight / weight;
					i().layout(i().rect.width, i().rect.height);
				}
			}

			/*
			 * Third pass, position all the elements.
			 */
			
			Element *next_line = element;
			element = line.start;
			margin_left = padding->left;
			int left = 0;

			while(element)
			{
				Extends margins(margin_left, margin_top, padding->right, padding->bottom, element->margins);
				
				left += margins.left;

				element->rect.left = left;
				element->rect.top = top + margins.top;

				left += element->rect.width;

				margin_left = element->margins->right;

				element = element->children_entry.next;
			}

			top += line.height;
			element = next_line;
		}
	}
};
