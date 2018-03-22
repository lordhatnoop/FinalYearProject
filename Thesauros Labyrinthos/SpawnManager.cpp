#include "SpawnManager.h"

SpawnManager::SpawnManager()
{
}

void SpawnManager::spawnEnemies(b2World &world,MazeGeneration maze,vector<std::shared_ptr<Skeleton>>& skeletonVector, vector<std::shared_ptr<Medusa>>& medusaVector, vector<std::shared_ptr<Griffin>>& griffinVector, vector<std::shared_ptr<Ghost>>& ghostVector)
{
	int numberOfEnemies = rand() % 30 + 10; //random number of enemies between 10 and 30
	
	for (int i = 0; i < numberOfEnemies;) { // loops
		int EnemyX = rand() % 99 + 1;
		int EnemyY = rand() % 59 + 1; //create X and Y postitons in the range of positions they can be. get rand to 99 or 59 becasue we want them to only be multiples of 10 so we take taht and multiply it by 10 


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
