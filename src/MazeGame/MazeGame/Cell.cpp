#include "Cell.h"

Cell::Cell() {
	Cell(0);
	bottom = top = left = right = false;
}

Cell::Cell(int t) {
	type = t;
	bottom = top = left = right = false;
}

int Cell::getType() {
	return type;
}

bool Cell::isEmpty() {
	return (getType() == 0);
}