#pragma once
#include "stdafx.h"
#include "Cell.h"
#include "WallCell.h"
#include "FloorCell.h"
class MazeGeneration {
public:
	MazeGeneration();
	void generateMaze();
	vector <Cell*> cellsVector;
	
	
	int startX;
	int startY;
private:
	int cellInRange(int x, int y);
	void digWall(int x0, int y0, int x1, int y1);
	void fillCells();
	void addRooms(int x, int y, bool posORNeg);
};