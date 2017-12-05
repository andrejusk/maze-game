#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include "Cell.h"
#include "Main.h"
#include "MapGenerator.h"
#include "Draw.h"

void showMainScreen();
void gameLoop();

/* Console display parameters */
const int CONSOLE_WIDTH = 119;
const int CONSOLE_HEIGHT = 29;
const int X_MARGIN = 2;
const int Y_MARGIN = 1;

/* Map variable */
static const int MAP_WIDTH = CONSOLE_WIDTH - 2 * X_MARGIN;
static const int MAP_HEIGHT = CONSOLE_HEIGHT - 2 * Y_MARGIN;
int map[MAP_HEIGHT][MAP_WIDTH];

/* Character properties, relative to Map not Console */
int x_pos, y_pos;

/* Movement keys */
const int KEYS[] = { 119, 97, 115, 100 };
const int ARROW[] = { 72, 75, 80, 77 };
enum Direction { Up, Left, Down, Right, None };

/* Map tiles */
enum MapTiles { Gem = -3, End = -2, Player = -1, Empty = 0, Black = 1, Dark = 10, Medium = 11, Light = 12, White = 13 };

/* Player score */
int score = 0;

/* Whether player has won */
bool won = 0;



/** Returns true if player is on end cell */
bool checkWin() {
	return (map[y_pos][x_pos] == -2);
}

/** Reads map, finds and sets player position */
void findPlayer() {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == -1) {
				/* Sets player position */
				x_pos = x;
				y_pos = y;
				/* Empties cell */
				map[y][x] = 0;
				return;
			}
		}
	}
}

/** Lights up nearby tiles of tile at given coordinates */
void lightUp(int py, int px, int level) {
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (map[py + y][px + x] >= Black) {
				if (map[py + y][px + x] == Black) {
					score += 10;
				}
				map[py + y][px + x] = level;
			}
		}
	}
}

/** Projects player vision in all directions */
void processLighting() {
	/* Resets cell light level to dark */
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] > Dark) {
				map[y][x] = Dark;
			}
		}
	}

	MapTiles level = Light;
	const int THRESHHOLD = 2;

	/* Projects downwards */
	for (int i = 0; i < MAP_WIDTH; i++) {
		if (map[y_pos + i][x_pos] > 0) {
			break;
		}
		if (i > 2 * THRESHHOLD) {
			level = Dark; 
		}
		else if (i > THRESHHOLD) {
			level = Medium;
		}
		else {
			level = Light;
		}
		lightUp(y_pos + i, x_pos, level);
	}
	/* Projects upwards */
	for (int i = 0; i > (0 - MAP_WIDTH); i--) {
		if (map[y_pos + i][x_pos] > 0) {
			break;
		}
		if (i < (0 - 2 * THRESHHOLD)) {
			level = Dark;
		}
		else if (i <  (0 - THRESHHOLD)) {
			level = Medium;
		}
		else {
			level = Light;
		}
		lightUp(y_pos + i, x_pos, level);
	}

	/* Projects right */
	for (int i = 0; i < MAP_WIDTH; i++) {
		if (map[y_pos][x_pos + i] > 0) {
			break;
		}
		if (i > 2 * THRESHHOLD) {
			level = Dark;
		}
		else if (i > THRESHHOLD) {
			level = Medium;
		}
		else {
			level = Light;
		}
		lightUp(y_pos, x_pos + i, level);
	}
	/* Projects left */
	for (int i = 0; i > (0 - MAP_WIDTH); i--) {
		if (map[y_pos][x_pos + i] > 0) {
			break;
		}
		if (i < (0 - 2 * THRESHHOLD)) {
			level = Dark;
		}
		else if (i < (0 - THRESHHOLD)) {
			level = Medium;
		}
		else {
			level = Light;
		}
		lightUp(y_pos, x_pos + i, level);
	}
	
	/* Highlights nearby cells */
	lightUp(y_pos, x_pos, White);
}

/** Returns true if exit state, otherwise processes input */
bool processInput(int key) {
	/* ESC pressed */
	if (key == 27) {
		return true;
	}

	/* User on endtile and wishes to exit */
	if (key == 13 && checkWin()) {
		won = true;
	}

	/* Determine direction pressed */
	Direction dir;
	for (int i = 0; i < 4; i++) {
		if (key == KEYS[i] || key == ARROW[i]) {
			dir = (Direction) i;
			break;
		}
		dir = None;
	}

	/* Movement */
	switch (dir) {
		case Up:
			if (map[y_pos - 1][x_pos] <= 0) {
				y_pos--;
			}
			break;
		case Down:
			if (map[y_pos + 1][x_pos] <= 0) {
				y_pos++;
			}
			break;
		case Left:
			if (map[y_pos][x_pos - 1] <= 0) {
				x_pos--;
			}
			break;
		case Right:
			if (map[y_pos][x_pos + 1] <= 0) {
				x_pos++;
			}
			break;
		default:
			break;
	}

	/* Gems */
	if (map[y_pos][x_pos] == -3) {
		score += 1000;
		map[y_pos][x_pos] = 0;
	}

	return false;

}



/** Prints help screen */
void showHelp() {
	/* Clear console screen before serving menu */
	system("cls");
	/* Help text */
	std::string help = "\n\n\n\n\n\n\n\
\n\t\t\t\tMove around the randomly generated maze by using WASD.\
\n\t\t\tEarn score by exploring the maze, collecting gems and reaching the exit.\
\n\t\tEach lit tile is worth 10 points, gems are worth 1,000 points, finishing earns 5,000 points.\
\n\t\t\t\
\n\t\t\t\tOnce on the exit tile, press <ENTER> to finish the maze.\
\n\t\t\tAt any point of the game, you may press <ESC> to go back to the main menu.\
\n\t\t\t\t\
\n\t\t\t\t\t\t* - The player (you!)\
\n\t\t\t\t\t\t# - End tile\
\n\t\t\t\t\t\t. - Gem\
\n\t\t\t\t\
\n\n\t\t\t\t\tPress any key to continue...";
	/* Prints help text */
	printf(help.c_str());
}

/** Returns true on exit, otherwise accepts user input for playing a given maze */
bool playMaze(int map[][MAP_WIDTH]) {
	/* Input variables */
	int key;

	/* Exit state */
	bool exit = false;

	/* Drawing */
	Draw draw;

	/* Game logic */
	while (true) {
		/* Vision */
		processLighting();

		/* (Re)draw maze */
		draw.draw(map, x_pos, y_pos, score, won);

		/* Wait for input */
		key = (int)_getch();

		/* Process input */
		if (processInput(key)) {
			return true;
		}

		/* Win state */
		if (won) {
			score += 5000;
			draw.draw(map, x_pos, y_pos, score, won);
			won = false;
			key = (int)_getch();
			break;
		}

	}

	return false;

}

/** Shows main menu and handles input */
void showMainScreen() {
	/* Input variable */
	int key;

	/* Map generator */
	MapGenerator generator;

	/* Menu text */
	std::string menu = "\
\t\t\t\t __  __                  _____\
\n\t\t\t\t|  \\/  |                / ____|\
\n\t\t\t\t| \\  / | __ _ _______  | |  __  __ _ _ __ ___   ___\
\n\t\t\t\t| |\\/| |/ _` |_  / _ \\ | | |_ |/ _` | '_ ` _ \\ / _ \\\
\n\t\t\t\t| |  | | (_| |/ /  __/ | |__| | (_| | | | | | |  __/\
\n\t\t\t\t|_|  |_|\\__,_/___\\___|  \\_____|\\__,_|_| |_| |_|\\___|\
\n\t\t\t\t\t\
\n\t\t\t\t\t\
\n\t\t\t\t\tWelcome to the Main Menu!\
\n\t\t\t\t\t\
\n\t\t\t\t\t<Enter>\t - Start Maze Game\
\n\t\t\t\t\t<F1>\t - Gameplay Instructions\
\n\t\t\t\t\t<F2>\t - Demo Maze Generation\
\n\t\t\t\t\t<Esc>\t - Exit Game\
\n\t\t\t\t\t\
\n\t\t\t\t\tSelection: ";

	do {
		/* Clear console screen before serving menu */
		system("cls");

		/* Top whitespace */
		for (int i = 0; i < (MAP_HEIGHT - 16) / 2; i++) {
			printf("\n");
		}

		/* Prints menu text */
		printf(menu.c_str());

		/* Waits for user input */
		key = (int)_getch();

		switch (key) {
			/* Gameplay help */
			case 59: 
				showHelp();
				key = (int)_getch();
				break;
			/* Demo maze generation */
			case 60:
				generator.produceMap(map, true);
				findPlayer();
				playMaze(map);
				break;
			/* Start game */
			case 13:
				score = 0;
				gameLoop();
				break;
			/* Exit */
			case 27:
				system("cls");
				exit(1);
				break;
		}

	} while (true);

}

/** Starts game loop */
void gameLoop() {

	while (true) {
		/* Generates map */
		MapGenerator generator;
		generator.produceMap(map, false);

		/* Spawn player */
		findPlayer();

		/* Run game logic */
		if (playMaze(map)) {
			return;
		}
	}

}



int main() {
	/* Displays main menu screen */
	showMainScreen();
	return 0;
}

