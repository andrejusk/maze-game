#pragma once
#include "MapGenerator.h"

class Draw {
	 
	private:
		static const int CONSOLE_WIDTH = 119;
		static const int CONSOLE_HEIGHT = 29;
		static const int X_MARGIN = 2;
		static const int Y_MARGIN = 1;
		static const int MAP_WIDTH = CONSOLE_WIDTH - 2 * X_MARGIN;
		static const int MAP_HEIGHT = CONSOLE_HEIGHT - 2 * Y_MARGIN;
	public:
		void draw(int map[][MAP_WIDTH], int x_pos, int y_pos, int score, bool won);

};