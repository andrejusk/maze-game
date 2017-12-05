#include "MapGenerator.h"
#include <vector>
#include "Draw.h"

/* Map tiles */
enum MapTiles { Gem = -3, End = -2, Player = -1, Empty = 0, Black = 1, Dark = 10, Medium = 11, Light = 12, White = 13 };

void MapGenerator::produceMap(int m[][MAP_WIDTH], bool demo) {
	generateMap(demo);
	translateMap();
	std::copy(&map[0][0], &map[0][0] + MAP_HEIGHT*MAP_WIDTH, &m[0][0]);
}



/** Returns true if map has univisited cells */
bool MapGenerator::mapHasUnvisited() {
	for (int y = 0; y < CELL_HEIGHT; y++) {
		for (int x = 0; x < CELL_WIDTH; x++) {
			if (!cellMap[y][x].visited) {
				return true;
			}
		}
	}
	return false;
}

/** Returns a random neighbouring cell of a cell*/
Cell MapGenerator::randomNeighbour(Cell cell) {
	int x, y, variable, amount, iteration = 0;
	bool found = false, valid, x_bounds, y_bounds;

	while (!found) {
		x = cell.x;
		y = cell.y;

		variable = std::rand() % 2;
		amount = (std::rand() % 2) * 2 - 1;

		if (variable == 1) {
			x += amount; 
		}
		else {
			y += amount; 
		}

		x_bounds = (x >= 0) && (x < (CELL_WIDTH));
		y_bounds = (y >= 0) && (y < (CELL_HEIGHT));
		valid = x_bounds && y_bounds;
		if (valid) {
			found = !cellMap[y][x].visited;
		}
	}
	
	return cellMap[y][x];
}

/** Returns true if cell has unvisited neighbouring cells */
bool MapGenerator::hasUnivisitedNeighbours(Cell cell) {
	int x = cell.x;
	int y = cell.y;

	bool found = false;
	bool top, bottom, left, right;
	top = bottom = left = right = false;

	if (y > 0) {
		top = !cellMap[y - 1][x].visited;
		found = found || top;
	}
	if (y < (CELL_HEIGHT - 1)) {
		bottom = !cellMap[y + 1][x].visited;
		found = found || bottom;
	}
	if (x > 0) {
		left = !cellMap[y][x - 1].visited;
		found = found || left;
	}
	if (x < (CELL_WIDTH - 1)) {
		right = !cellMap[y][x + 1].visited;
		found = found || right;
	}

	return found;
}

/** Removes wall inbetween two cells */
void MapGenerator::removeWall(Cell cell1, Cell cell2) {

	int differenceX = cell1.x - cell2.x;
	int differenceY = cell1.y - cell2.y;

	/* Cell1 to the right of Cell2 */
	if (differenceX == 1) {
		cellMap[cell1.y][cell1.x].left = true;
		cellMap[cell2.y][cell2.x].right = true;
	}
	/* Cell1 to the left of Cell2 */
	else if (differenceX == -1) {
		cellMap[cell1.y][cell1.x].right = true;
		cellMap[cell2.y][cell2.x].left = true;
	}
	/* Cell1 below Cell2 */
	else if (differenceY == -1) {
		cellMap[cell1.y][cell1.x].bottom = true;
		cellMap[cell2.y][cell2.x].top = true;
	}
	/* Cell1 above Cell2 */
	else if (differenceY == 1) {
		cellMap[cell1.y][cell1.x].top = true;
		cellMap[cell2.y][cell2.x].bottom = true;
	}

}



/** Generates a cellmap based on the recursive backtracker algorithm */
void MapGenerator::generateMap(bool demo) {
	/* Sets up every cell */
	for (int y = 0; y < CELL_HEIGHT; y++) {
		for (int x = 0; x < CELL_WIDTH; x++) {
			cellMap[y][x] = Cell();
			cellMap[y][x].visited = false;
			cellMap[y][x].x = x;
			cellMap[y][x].y = y;
		}
	}

	/* Generate start and end points until they are different */
	do {
		start_x = std::rand() % CELL_WIDTH;
		start_y = std::rand() % CELL_HEIGHT;
		end_x = std::rand() % CELL_WIDTH;
		end_y = std::rand() % CELL_HEIGHT;
	} while (start_x == end_x && start_y == end_y);

	int rand_x, rand_y;
	/* Gem points */
	for (int i = 0; i < 15; i++) {
		rand_x = std::rand() % CELL_WIDTH;
		rand_y = std::rand() % CELL_HEIGHT;
		cellMap[rand_y][rand_x].type = 1;
	}

	/* Initial cell */
	std::vector<Cell> stack;
	Cell currentCell = cellMap[start_y][start_x];
	cellMap[start_y][start_x].visited = true;

	stack.push_back(currentCell);

	bool wentBack;
	Draw draw;

	/* While map has unvisited cells */
	while (mapHasUnvisited()) {
		wentBack = false;
		/* Set current cell as visited */
		cellMap[currentCell.y][currentCell.x].visited = true;

		/* If cell has unvisited neighbours */
		if (hasUnivisitedNeighbours(currentCell)) {
			/* Set current cell as temporary cell */
			Cell tempCell = currentCell;
			/* Find new neighbour cell */
			currentCell = randomNeighbour(currentCell);
			stack.push_back(currentCell);
			removeWall(currentCell, tempCell);
		}

		/* If cell has no unvisited neighbours */
		else {
			/* Set current cell as previous cell */
			Cell tempCell = currentCell;
			currentCell = stack.back();
			stack.pop_back();
			wentBack = true;
		}

		if (demo && !wentBack) {
			translateMap();
			demoMap();
			draw.draw(map, -1, -1, -1, false);
		}

	}
	
	if (demo) {
		translateMap();
	}

}

/** Translate generated cellmap into map for maze game */
void MapGenerator::translateMap() {
	/* Walls entire map */
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			map[y][x] = Black;
		}
	}

	/* Processes Cells */
	for (int y = 0; y < CELL_HEIGHT; y++) {
		for (int x = 0; x < CELL_WIDTH; x++) {

			/* Default empty */
			map[y * 2 + 1][x * 2 + 1] = Empty;

			/* Empty Cell */
			if (cellMap[y][x].isEmpty()) {
				map[y * 2 + 1][x * 2 + 1] = Empty;
			}

			/* Special tiles */
			if (cellMap[y][x].type == 1) {
				map[y * 2 + 1][x * 2 + 1] = Gem;
			}
			if (y == start_y && x == start_x) {
				map[y * 2 + 1][x * 2 + 1] = Player;
			}
			if (y == end_y && x == end_x) {
				map[y * 2 + 1][x * 2 + 1] = End;
			}


			/* Empty adjacent cells */
			if (cellMap[y][x].top) {
				map[y * 2 - 1 + 1][x * 2 + 1] = Empty;
			}
			if (cellMap[y][x].left) {
				map[y * 2 + 1][x * 2 - 1 + 1] = Empty;
			}
			if (cellMap[y][x].bottom) {
				map[y * 2 + 1 + 1][x * 2 + 1] = Empty;
			}
			if (cellMap[y][x].right) {
				map[y * 2 + 1][x * 2 + 1 + 1] = Empty;
			}

		}
	}

}

/** Removes fog of war for demo purposes */
void MapGenerator::demoMap() {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] > Empty) {
				map[y][x] = White;
			}
			if (map[y][x] < Empty) {
				map[y][x] = Empty;
			}
		}
	}
}