
#ifndef MAIN_TOOL_BAR_H
# define MAIN_TOOL_BAR_H

# include "envHandler.h"

class MainBar
{
	private:
		bool hovered = false;
	public:
		bool IsHovered();
		void UpdateMainTools();
};

#endif
