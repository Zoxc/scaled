#include "stack.hpp"

namespace River
{
	void Stack::layout(int available_width, int available_height)
	{
		Element *element = children.first;
		int height = available_height;
		int margin_top = padding->top;
		ExtendList extends;
		Element *line_start = element;
		Element *last_visible = 0;
		
		while(element)
		{
			Extends margins(padding->left, margin_top, padding->right, padding->bottom, element->margins);
			
			int element_width = std::max(available_width - (margins.left + margins.right), 0);

			/*
			 * Delay layout of height extended elements until we know the actual size of it.
			 */
			if(element->height == Flags::Extend)
			{
				/*
				 * Use the minimum size as a height placeholder in calculations.
				 */
				element->rect.width = element_width;
				element->rect.height = element->min_height;
				
				/*
				 * Add vertically extended elements to the list.
				 */
				extends.append(element);
			}
			else
			{
				int element_height = std::max(height - (margins.top + margins.bottom), 0);

				element->layout(element_width, element_height);
			}

			int element_span = element->rect.height + margins.top;

			height -= element_span;
			margin_top = element->margins->bottom;

			last_visible = element;
			
			element = element->children_entry.next;
		}
			
		/*
		 * Include the bottom margin of the last element in the height calculation.
		 */
		if(last_visible)
			height -= std::max(padding->bottom, last_visible->margins->bottom);
		else
			height -= padding->bottom;

		/* 
		 * Second pass, extend elements vertically.
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
				i().rect.height += height * i().weight / weight;
				i().layout(i().rect.width, i().rect.height);
			}
			
			/* 
			 * We used up all the height now.
			 */
			height = 0;
		}

		/*
		 * Third pass, position all the elements.
		 */

		Element *next_line = element;
		element = line_start;
		margin_top = padding->top;
		int max_width = 0;
		int top = 0;

		while(element)
		{
			Extends margins(padding->left, margin_top, padding->right, padding->bottom, element->margins);
			
			top += margins.top;

			element->rect.left = margins.left;
			element->rect.top = top;

			top += element->rect.height;

			margin_top = element->margins->bottom;

			max_width = std::max(max_width, margins.left + element->rect.width + margins.right);

			element = element->children_entry.next;
		}

		simple_layout(available_width, available_height, max_width, available_height - height);
	}
};
