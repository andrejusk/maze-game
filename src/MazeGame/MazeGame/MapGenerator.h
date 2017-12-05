#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "Cell.h"
#include "Main.h"

class MapGenerator {

	private:
		bool mapHasUnvisited();
		Cell randomNeighbour(Cell cell);
		bool hasUnivisitedNeighbours(Cell cell);
		void removeWall(Cell cell1, Cell cell2);

		void generateMap(bool demo);
		void translateMap();
		void demoMap();

		static const int CONSOLE_WIDTH = 119;
		static const int CONSOLE_HEIGHT = 29;
		static const int X_MARGIN = 2;
		static const int Y_MARGIN = 1;

		static const int MAP_WIDTH = CONSOLE_WIDTH - 2 * X_MARGIN;
		static const int MAP_HEIGHT = CONSOLE_HEIGHT - 2 * Y_MARGIN;
		static const int CELL_WIDTH = (MAP_WIDTH - 1) / 2;
		static const int CELL_HEIGHT = (MAP_HEIGHT - 1) / 2;

		Cell cellMap [CELL_HEIGHT][CELL_WIDTH];
		int map [MAP_HEIGHT][MAP_WIDTH];

		int start_x, start_y, end_x, end_y;

	public:
		void produceMap(int map[][MAP_WIDTH], bool demo);

};