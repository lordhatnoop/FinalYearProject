#pragma once
#include "stdafx.h"
#include "Cell.h"
#include "WallCell.h"
#include "FloorCell.h"
#include "ExitCell.h"
#include "NonBodyWallCell.h"
#include "FakeWallCell.h"

//changing the values of these defines changes the eventual outcome of the maze generation , experiment. (e.g changing radius to 1 result in more of a cave than a maze.
//size of the map
#define maze_size_x 100 //can be anything - 80 and 45 for y works well. if change this, need to change the loadEscapeLevel load thing too (has counters for this since it doesn't create a new maze and so doesn't access this.
#define maze_size_y 60

#define cell_Radius 2

#define MAGIC 666

#define max_Cell_Width (maze_size_x / cell_Radius -1)
#define max_Cell_Height (maze_size_y / cell_Radius -1 )

class MazeGeneration {
public:
	MazeGeneration();
	void generateMaze(b2World &world);
	vector <std::shared_ptr<Cell>> cellsVector;
	/*
	vector <WallCell> wallCellVector;
	vector <FakeWallCell> fakeWallCellVector;
	vector <FloorCell> floorCellVector;
	vector <NonBodyWallCell> nonBodyWallCellVector;
	vector <ExitCell> exitCellVector;
	*/
	
	int levelCounter;

	// This saves us from making a bunch of similar conditionals for checking directions 
	int directionList[4][2] = { { 0, -1 },{ 1, 0 },{ 0, 1 },{ -1, 0 } };

	//create maze and cell arrays.
	int maze[maze_size_x][maze_size_y];
	int cell[max_Cell_Width][max_Cell_Height];

	int mazeChangeArray[maze_size_x][maze_size_y]; // cell array used for changing the maze when we are still checking it, so that the changes don't affect the check


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