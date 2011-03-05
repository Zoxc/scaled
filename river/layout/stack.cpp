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
				/*
				 * If the element is not width extended, layout it now.
				 /
				if(i().width != Flags::Extend)
				{
					i().layout(i().rect.width, i().rect.height);
						
					/*
						* Update line height and margin if needed.
						/
					int element_span = i().rect.height + std::max(margin_top, i().margins->top);

					line_height = std::max(line_height, element_span);
					line_margin_bottom = std::max(line_margin_bottom, element_span + i().margins->bottom);
				}*/
			}

			height = 0;
		}

		/*
			* Height is calculated. Adjust margins.
			
		line_margin_bottom -= line_height;

		/*
			* Third pass, extend elements vertically and position all the elements.
			*/

		Element *next_line = element;
		element = line_start;
		margin_top = padding->top;
		int top = 0;

		while(element)
		{
			Extends margins(padding->left, margin_top, padding->right, padding->bottom, element->margins);
			
			top += margins.top;

			element->rect.left = margins.left;
			element->rect.top = top + margins.top;

			top += element->rect.height;

			margin_top = element->margins->bottom;

			element = element->children_entry.next;
		}

		rect.width = available_width;
		rect.height = available_height - height;
	}
};
