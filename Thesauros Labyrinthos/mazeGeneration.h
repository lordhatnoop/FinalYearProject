#pragma once
#include "stdafx.h"
#include "Cell.h"
#include "WallCell.h"
#include "FloorCell.h"
#include "ExitCell.h"
#include "NonBodyWallCell.h"
#include "FakeWallCell.h"
#include "Node.h"

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
	vector < vector<int>> adjacency;
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
	int endX = 0;
	int endY = 0;

	//path gen stuff
	int indexLookup[maze_size_x][maze_size_y];
	int nNodes = maze_size_x * maze_size_y; // number of nodes/cells
	short index(short x, short y) { return indexLookup[x][y]; }
	bool inverseIndex(short index, short *x, short *y) {
		if (index > nNodes) { return false; }
		else
		{
			*x = index % maze_size_x;
			*y = index / maze_size_x;
			return true;
		}
	};

	list<int> GeneratePath(short currentI, short currentJ, short goalI, short goalJ)
	{
		Node currentNode;
		list <int>path;
		list<Node> open; // List of open nodes to be explored
		list<Node> closed; // List of closed nodes which have been explored
		int goalIndex; // Index of the goal node
		int currentIndex; // Index of the current node

		goalIndex = index(goalI, goalJ); // Set goal node index
		currentIndex = index(currentI, currentJ); // Set current node index
		currentNode.index = currentIndex; // Set the index of the currentgraph node
		currentNode.parentIndex = -1; // Set the parent of the current node to NULL
		currentNode.score(0, currentI, currentJ, goalI, goalJ); // Score the current node
		open.push_back(currentNode); // Put the current node on the open list

		while (currentIndex != goalIndex)
		{

			if (open.size() >= 1) {
				open.sort();	// Sort the list of open node by list quality

				currentNode = open.front();	// Take the node that scored best on the open list
				open.pop_front();// Take it off the open list
				closed.push_back(currentNode);// Add that node to the closed list
				currentIndex = currentNode.index;// Set the current index
			}
											 // Look in the adjacency matrix for connections to other nodes
			for (int otherIndex = 0; otherIndex < nNodes; otherIndex++) {
				//check for a connection 
				if (adjacency[currentIndex][otherIndex]) {
					//if there is a connection
					list<Node>::iterator graphListIter;
					bool onClosed = false;
					bool onOpen = false;

					//find if it's on the closed list
					//check through closed using graphiterator until reaching the end of closed
					for (graphListIter = closed.begin(); graphListIter != closed.end(); ++graphListIter) {

						//if index's are same
						if ((*graphListIter).index == otherIndex) {
							//found on closed list
							onClosed = true;

							//check g value 
							//if current g value is less than that of the node found on the current node index
							//then update that nodes parent to the current node
							if (currentNode.gCost + 1.0 < (*graphListIter).gCost) {
								(*graphListIter).parentIndex = currentIndex;
							}
						}
					}

					//if not closed
					if (!onClosed) {
						//find out whether it's on the open list
						for (graphListIter = open.begin(); graphListIter != open.end(); ++graphListIter) {

							if ((*graphListIter).index == otherIndex) {

								//found on open list
								onOpen = true;

								//check g value 
								//if current g value is less than that of the node found on the current node index
								//then update that nodes parent to the current node
								if (currentNode.gCost + 1.0 < (*graphListIter).gCost) {
									(*graphListIter).parentIndex = currentIndex;
								}
							}
						}
					}


					//if it's not in either
					if (!onClosed && !onOpen) {
						//if not on open or closed
						Node tmpNode;

						//set parent node to the current node
						tmpNode.parentIndex = currentIndex;
						//set node index to other index
						tmpNode.index = otherIndex;
						//find the x and y position of that node 
						inverseIndex(otherIndex, &currentI, &currentJ);
						//score the node
						tmpNode.score(currentNode.gCost, currentI, currentJ, goalI, goalJ);
						//add to open list
						open.push_back(tmpNode);
					}

					//Check if adjacent node on closed list, open list, and see if $g$ can be improved
					// If not set values for this node and add to open list.

				}

			}
		}
		// Reconstruct the path
		// Start with goal node working backwards towarsd the first node
		// We know that we’re at the first node because it has a parent of -1
		int parent;
		list <Node>::iterator graphListIter;

		if (closed.size() >= 1) {
			currentNode = closed.back(); //set current node to  goal node
			parent = currentNode.parentIndex;// set the parent to be the parent of the current node
			path.push_front(currentIndex); //add current node to the satr of the path
			closed.pop_back(); // removes the node from the back of closed

						   //Now work backwards throught the closed list reconstructing the path

			for (graphListIter = closed.end(), graphListIter--; graphListIter != closed.begin(); --graphListIter) {
				currentNode = *graphListIter;
				if (currentNode.index == parent) //found the node
				{
					//add to the path
					path.push_front(parent);
					//set parent to be this nodes parent
					parent = currentNode.parentIndex;
					//remove node from the closed list
					closed.erase(graphListIter);
					//start working backward through the closed again from the end
					graphListIter = closed.end();
				}
			}
		}
		return path;

	};

private:
	int cellInRange(int x, int y);
	void digWall(int x0, int y0, int x1, int y1);
	//void fillCells();
	void genEndPos();
	void addRooms();
	void setAdjacencySize();
	void createAdjacency(int j, int i,int position);
	void populateIndex();
};