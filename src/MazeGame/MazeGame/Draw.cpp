#include "Draw.h"

/*

(char) codes
0 -		0 - black (empty)
1 -		0 - black (wall)
10	- 176 - dark grey
11	- 177 - medium grey
12	- 178 - light grey
13	- 219 - white

*/

/* Map tiles */
enum MapTiles { Gem = -3, End = -2, Player = -1, Empty = 0, Black = 1, Dark = 10, Medium = 11, Light = 12, White = 13 };

/** Retuns printing value for tile */
char parseTile(int tile) {
	switch (tile) {
		case Gem:
			return '.';
		case End:
			return '#';
		case Empty:
			return ' ';
		case Black:
			return ' ';
		case Dark:
			return (char)176;
		case Medium:
			return (char)177;
		case Light:
			return (char)178;
		case White:
			return (char)219;
		default:
			return 'X';
	}
}

/** Draws maze */
void Draw::draw(int map[][MAP_WIDTH], int x_pos, int y_pos, int score, bool won) {
	/* Output string and temporary character */
	std::string output = "";
	std::string temp = "";

	/* Top margin */
	for (int i = 0; i < (Y_MARGIN); i++) {
		output = output + "\n";
	}
	
	/* Score display */
	if (score != -1) {
		output = output + "Score: " + std::to_string(score) + "\n";
	}
	else {
		output = output + "\n";
	}

	/* For every row */
	for (int y = 0; y < MAP_HEIGHT; y++) {

		/* Left margin */
		for (int i = 0; i < X_MARGIN; i++) {
			output = output + " ";
		}

		/* For every column */
		for (int x = 0; x < MAP_WIDTH; x++) {

			/* Prints game win menu */
			if (won && x == (MAP_WIDTH / 2 - 25)) {
				/* Borders */
				if (y == (MAP_HEIGHT / 2)) {
					output = output + (char)201;
					for (int i = 0; i < 48; i++) {
						output = output + (char)205;
					}
					output = output + (char)187;
					x += 50;
				}
				/* Win text */
				else if (y == ((MAP_HEIGHT / 2) + 1)) {
					output = output + (char)186 + " Maze complete. Press any key to continue on... " + (char)186;
					x += 50;
				}
				else if (y == (MAP_HEIGHT / 2 + 2)) {
					output = output + (char)200;
					for (int i = 0; i < 48; i++) {
						output = output + (char)205;
					}
					output = output + (char)188;
					x += 50;
				}
			}

			/* Player position check */
			if (x == x_pos && y == y_pos) {
				output = output + "*";
			}
			/* Tile parsing */
			else {
				output = output + parseTile(map[y][x]);
			}
		}
		output = output + "\n";
	}
	/* Bottom margin */
	output = output + "\n";

	/* Output to console */
	std::printf(output.c_str());
	std::fflush(stdout);
}