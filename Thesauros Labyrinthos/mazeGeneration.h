#pragma once
#include "stdafx.h"
#include "Cell.h"
#include "WallCell.h"
#include "FloorCell.h"
#include "ExitCell.h"
#include "NonBodyWallCell.h"

class MazeGeneration {
public:
	MazeGeneration();
	void generateMaze(b2World &world);
	vector <Cell*> cellsVector;
	
	//ints to hold the x and y of the first cell so we can use that as the start for the player
	int startX;
	int startY;

	//hold the last cell for the exit
	int endX;
	int endY;


private:
	int cellInRange(int x, int y);
	void digWall(int x0, int y0, int x1, int y1);
	void fillCells();
	void addRooms(int x, int y, bool posORNeg);
};