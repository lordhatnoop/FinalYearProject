#include "SpawnManager.h"

SpawnManager::SpawnManager()
{
}

void SpawnManager::spawnEnemies(b2World &world,MazeGeneration maze,vector<std::shared_ptr<Skeleton>>& skeletonVector, vector<std::shared_ptr<Medusa>>& medusaVector, vector<std::shared_ptr<Griffin>>& griffinVector, vector<std::shared_ptr<Ghost>>& ghostVector)
{
	int numberOfEnemies = rand() % 30 + 10; //random number of enemies between 10 and 30
	
	for (int i = 0; i < numberOfEnemies;) { // loops
		int EnemyX = rand() % 80 + 1;
		int EnemyY = rand() % 50 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 


		if (maze.maze[EnemyX][EnemyY] == 0) { // if it's a floor tile
			int enemyType = rand() % 4 + 1; // random enemy type

			if (enemyType == 1) {
				skeletonVector.push_back(std::shared_ptr<Skeleton>(new Skeleton(EnemyX * 10, EnemyY* 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
				skeletonVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. 
			}
			else if (enemyType == 2) {
				medusaVector.push_back(std::shared_ptr<Medusa>(new Medusa(EnemyX * 10, EnemyY * 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
				medusaVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. 
			}
			else if (enemyType == 3) {
				griffinVector.push_back(std::shared_ptr<Griffin>(new Griffin(EnemyX * 10, EnemyY * 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
				griffinVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. progress i here so that we're guarenteed right number of enemies
			}
			else if (enemyType == 4) {
				ghostVector.push_back(std::shared_ptr<Ghost>(new Ghost(EnemyX * 10, EnemyY * 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
				ghostVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. 
			}
		}
	}
	//int EnemyX = rand() % 99 + 1;
//	int EnemyY = rand() % 59 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 

	//griffinVector.push_back(std::shared_ptr<Griffin>(new Griffin(EnemyX * 10, EnemyY * 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
	//griffinVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
	//ghostVector.push_back(std::shared_ptr<Ghost>(new Ghost(EnemyX * 10, EnemyY * 10))); //create a new enemy at that point if it's floor . multiply the pos's by 10 to get the actual position we wanted
	//ghostVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
}

void SpawnManager::spawnTreasure(b2World & world, MazeGeneration maze, vector<std::shared_ptr<Treasure>>& treasureVector)
{
	int numberOfTreasure = rand() % 60 + 40; //random number of enemies between 40 and 60


	for (int i = 0; i < numberOfTreasure;) { // loops
		int treasureX = rand() % 80 + 1;
		int treasureY = rand() % 50 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 

		if (maze.maze[treasureX][treasureY] == 0) { // if it's a floor tile
			i++; //update i

			treasureVector.push_back(std::shared_ptr<Treasure>(new Treasure(treasureX, treasureY)));
			treasureVector.back()->CreateBody(world);
		}
	}
}

void SpawnManager::spawnChests(b2World & world, MazeGeneration maze, vector<std::shared_ptr<TreasureChest>> treasureChestVector)
{
	int numberOfChests = rand() % 10 + 1; //between 1 and 10

	for (int i = 0; i < numberOfChests;) {//loops
		int treasureX = rand() % 80 + 1;
		int treasureY = rand() % 50 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 

		if (maze.maze[treasureX][treasureY] == 0) { // if it's a floor tile
			i++; //update i

			treasureChestVector.push_back(std::shared_ptr<TreasureChest>(new TreasureChest(treasureX, treasureY)));
			treasureChestVector.back()->createBody(world);
		}
	}
}
void SpawnManager::spawnTraps(b2World & world, MazeGeneration maze, vector<std::shared_ptr<GameTraps>> trapsVector)
{
	int numberOfTraps = rand() % 10 + 1; //between 1 and 10

	for (int i = 0; i < numberOfTraps;) {//loops
		int trapX = rand() % 80 + 1;
		int trapY = rand() % 50 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 

		if (maze.maze[trapX][trapY] == 0) { // if it's a floor tile
			i++; //update i

			int trapType = rand() % 30 + 1; //between 30 and 1

			if (trapType >= 1 && trapType <= 15) { // if between 1 and 15 - spike trap
				trapsVector.push_back(std::shared_ptr<SpikeTrap>(new SpikeTrap(trapX, trapY))); //create trap
				trapsVector.back()->createBox2D(world);
			}
			else if (trapType > 15 && trapType <= 25) { // 16 to 25 - arrow trap
				trapsVector.push_back(std::shared_ptr<ArrowTrap>(new ArrowTrap(trapX, trapY))); //create trap
				trapsVector.back()->createBox2D(world);
			}
			else { //the rest - boulder idol trap
				trapsVector.push_back(std::shared_ptr<IdolBoudlerTrap>(new IdolBoudlerTrap(trapX, trapY))); //create trap
				trapsVector.back()->createBox2D(world);
			}
		}
	}
}

