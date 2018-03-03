#include <time.h>
#include <vector>
#include <list>
#include "mazeGeneration.h"

using namespace std;


MazeGeneration::MazeGeneration() {
	
}

//llok through tank wars (adjacency!)
void MazeGeneration::generateMaze(b2World &world) {

	//set srand to be time(0) this effectivly generates a random seed for the maze because the way rand() works means calling srand and passing it the current time (which will always be different) changes how rand will select values
	//changing how the maze is generated.
	srand(time(0));
	//int temp = maze_size_x / cell_Radius - 1;
	//counters
	int i;
	int j;

	//initialise the grid. set the whole maze array to be one
	for (i = 0; i < maze_size_x; i++) {
		for (j = 0; j < maze_size_y; j++) {
			maze[i][j] = 1;
		}
	}
	
	//similarly init cells
	for (i = 0; i < max_Cell_Width; i++) {
		for (j = 0; j < max_Cell_Height; j++) {
			cell[i][j] = 0;
		}
	}

	//setup some ints
	int rx = 0; int ry = 0;
	int dx; int dy;
	int dir = 0;
	int count = 0;

	//get the total amount of cells
	int totalCells = max_Cell_Width * max_Cell_Height ;

	//setup the start cell position
	rx = max_Cell_Width / 2;
	ry = max_Cell_Height / 2;

	//set the start cell to visited
	cell[rx][ry] = 1;
	//store the start position so it can be used for player (easiest to place player if we know the point they're at is definatly going to be a floor
	startX = rx * 20;
	startY = ry* 20;


	//keep track of how many cells we've visted so that we can use it in the loop
	int visitedCells = 1; // The Cell we just assigned to 1!

	// backtracking style of maze genration, keeps choosing random cells next to current one until out of space, then backtracks to an already visited cell and carries on. only difference from traditional is the backtracked cell is any visted cell picked 
	//at random and the maze generation goes until reaching a goal amount of visted cells (including revisiting the same cell), rather than visiting them all. This gives it more of a level design and less of a traditional maze layout, while maintaining maze charcterisitics
	while (visitedCells < totalCells) {

		count++;
		if (count > MAGIC) {
			fillCells();
		}


		// use the direction table at the top for the dig function. choose which one useing rand to generate random number between 1 and 4
		dir = rand() % 4;

		dx = directionList[dir][0];
		dy = directionList[dir][1];

		//checks whether the target wall to be dug is free using in range to check it's in range and then checking if the cell is a 0.
		//if it is digs the target cell/wall by calling digwall
		//digs the wall even if the cell isn't available sometimes because of the end biut of the condition which says "or if random number out of 7 is 6"
		//do this to add some loops to the map and not have a perfect maze (maze with only one correct passage)
		if (cellInRange(rx * cell_Radius + dx, ry * cell_Radius + dy) == 1 && cell[rx + dx][ry + dy] == 0 ) {
			digWall(rx * cell_Radius, ry * cell_Radius, rx + dx * cell_Radius, (ry + dy) * cell_Radius);
			rx += dx;
			ry += dy;
		}
		//if can't dig chooses a random tile that has already been set to be a floor
		else {
			do {
				rx = rand() % max_Cell_Width;
				ry = rand() % max_Cell_Height;
				
			} while (cell[rx][ry] != 1);
		}

		//set this cell visited
		cell[rx][ry] = 1;

		maze[rx * cell_Radius][ry * cell_Radius] = 0;

		//update visited cells
		visitedCells++;

		//so long as it's a floor tile, store's it as the current last tile so that we can use it for the exit if needed
		if (maze[rx][ry] == 0 ) {
			endX = rx *20;
			endY = ry *20;
		}
	}

	//fillCells();

	//below is attempt at adding rooms. loop through maze, check if current maze spot is a floor, if passage and the spots around it are walls, chance to add room
	//refine this
	//isn't checking the long passages on the right for some reason
	
/*	for (i = 0; i < maze_size_y; i++) {
		for (j = 0; j < maze_size_x; j++) {
			//if passage
			if (maze[i][j] == 0) {
				//if the next two x along are passage
				if (maze[i+1][j] == 1 && maze[i+2][j] == 1){
					//if the space below is wall
					if (maze[i][j + 1] == 1) {
						//10 % chance to add room
						//if (rand() % 1 == 10) {
							addRooms(i, j, 1);
						//}
					}
				}
				if (maze[i - 1][j] == 1 && maze[i - 2][j] == 1) {
					//if the space below is wall
					if (maze[i][j + 1] == 1) {
						//10 % chance to add room
						//if (rand() % 1 == 10) {
							addRooms(i, j, 0);
						//}
					}
				}
			}
		}
	}*/
	//addRooms(40, 21, 1);

	//attempt at adding ways through the long passage stretchs
	for (i = 0; i < maze_size_x; i++) {
		for (j = 0; j < maze_size_y; j++) {
			//if passage
			if (maze[i][j] == 0 && maze[i +1][j] == 0 && maze[i+2][j] == 0 && maze[i+3][j] == 0 && maze[i+4][j] == 0 && maze[i+5][j] == 0 && maze[i+6][j] == 0 && maze[i + 7][j] == 0
				&& maze[i + 8][j] == 0 && maze[i + 9][j] == 0 && maze[i + 10][j] == 0 && maze[i + 11][j] == 0 && maze[i + 12][j] == 0) {
				//random chance, if it's 9 or 10 will dig. splits up the corridors some more, but experiment with higher numbers to get better maze generation
				if (rand() % 10 >= 9) {
					//change above and below to be floor
					
						maze[i][j + 1] = 0;
						maze[i][j - 1] = 0;
					
					
					

					//see if we can fill in the passages
				//maze[i + 1][j] = 1;
				//maze[i + 2][j] = 1;
				//maze[i + 3][j] = 1;
				//maze[i + 4][j] = 1;
				}
			}

			//rand for adding blocks above// experiment with this more.
			if (rand() % 100 > 90) {
				
					maze[i][j + 1] = 4; // adds random blocks into the maze above other blocks, randomising it a bit
				
			}

			
			//we are creating too many bodies having everything have one. check for walls that are completely surronded so that we can create "fake walls", wall cells without the box2d body
			if (maze[i][j + 1] == 1 
				&& maze[i + 1][j] == 1 
				&& maze[i][j - 1] == 1 
				&& maze[i - 1][j] == 1 ) {
				
				//set the temp to be 3 so that we don't change the current maze while we're still checking it
				//maze[i][j] = 3;
				mazeChangeArray[i][j] = 3;
			}
			
			

		}
	}
	//if the mazeChange array says to change something,m change it
	for (i = 0; i < maze_size_x; i++) {
		for (j = 0; j < maze_size_y; j++) {
			if (mazeChangeArray[i][j] == 3) {
				maze[i][j] = 3;
			}
		}
	}
	maze[startX / 20][startY / 20] = 0; // amke sure the start is always a floor
	maze[endX/ 20][endY / 20] = 2; // set the end to be 2 at the end of all the other generation to guarentee it's the exit

		// Done, so create the wallCells from the mazecells
		for (size_t x = 0; x < maze_size_x; x++) {
			for (size_t y = 0; y < maze_size_y; y++)
			{
				//if maze cell is 1 create wall
				if (maze[x][y] == 1) {
					printf("#");
					cellsVector.push_back(std::shared_ptr<WallCell>(new WallCell(x * 10, y * 10, world)));
					
				}
				else if (maze[x][y] == 0 ){
					printf("."); // create floorcell
					cellsVector.push_back(std::shared_ptr<FloorCell>(new FloorCell(x * 10, y * 10)));
				}
				else if (maze[x][y] == 2) {
					printf("2"); //otherwise create floorcell
					//cellsVector.push_back(new ExitCell(x * 10, y * 10));
				}
				else if (maze[x][y] == 3) { // if 3, the cell is completely surronded by walls, so create a nonbodyWallcell
					printf("3");
					cellsVector.push_back(std::shared_ptr<NonBodyWallCell>(new NonBodyWallCell(x * 10, y * 10)));
				}
				else if(maze[x][y] == 4) { // if 4, the cell is a fake wall created during the random extra walls.
					printf("4");
					cellsVector.push_back(std::shared_ptr<FakeWallCell>(new FakeWallCell(x * 10, y * 10,world)));
				}
			}
		}


	}



//below is used to return 1 if the cell we are checking is in range(doesn't go off the edge of maze and isn't too close to current cell so they overlap).
int MazeGeneration::cellInRange(int x, int y)
{
	if (x > 2 && y > 2 && x < maze_size_x - 2 && y < maze_size_y - 2) {
		return 1;
	}
	return 0;
}

//function to "link the cells" or "dig tunnles". gets current cell position and target cell position , checks if target cell is acceptable by calling inrange and if it is sets that maze spot to be a a 0 (passage)
//loops through by going until the cell x and y equal the passed target x and y.
void MazeGeneration::digWall(int x0, int y0, int x1, int y1)
{
	int cellX = x0; int cellY = y0;
	while (cellX != x1) {
		if (x0 > x1)
			cellX--;
		else
			cellX++;
		if (cellInRange(cellX, cellY) == 1)
			maze[cellX][cellY] = 0;
	}
	while (cellY != y1) {
		if (y0 > y1)
			cellY--;
		else
			cellY++;
		if (cellInRange(cellX, cellY) == 1)
			maze[cellX][cellY] = 0;
	}
}

//can't tell if this actually does anything? 
void MazeGeneration::fillCells()
{
	int i, j;
	for (i = 0; i < max_Cell_Width; i++) {
		for (j = 0; j < max_Cell_Height; j++) {
			if (cell[i][j] == 1)
				maze[i * cell_Radius][j * cell_Radius] = 0;
		}
	}
}

void MazeGeneration::addRooms(int x, int y, bool posORNeg )
{
	//below should theoretically take 6 cells near the passed xy and y and make them into floors to create a room
	if (posORNeg == 1) {
		maze[x + 1][y] = 0;
		maze[x + 2][y] = 0;
		maze[x + 1][y + 1] = 0;
		maze[x + 2][y + 1] = 0;
		maze[x + 1][y + 2] = 0;
		maze[x + 2][y + 2] = 0;
	}
	else if (posORNeg == 0) {
		maze[x - 1][y] = 0;
		maze[x - 2][y] = 0;
		maze[x - 1][y + 1] = 0;
		maze[x - 2][y + 1] = 0;
		maze[x - 1][y + 2] = 0;
		maze[x - 2][y + 2] = 0;
	}
	
}


