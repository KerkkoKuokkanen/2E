
#ifndef MAIN_TOOL_BAR_H
# define MAIN_TOOL_BAR_H

# include "envHandler.h"

class MainBar
{
	private:
		bool hovered = false;
		void CreateWindow();
		void LoadRoomMBar();
		void RoomSwitchMBar();
	public:
		bool IsHovered();
		void UpdateMainTools();
};

#endif
