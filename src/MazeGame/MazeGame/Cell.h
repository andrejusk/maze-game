#pragma once

class Cell {
	public:
		Cell();
		Cell(int t);
		bool isEmpty();
		int getType();

		int type;
		int x, y;
		bool top, right, bottom, left;
		bool visited;
};