#pragma once
#include "stdafx.h"
#include "skeleton.h"
#include "Medusa.h"
#include "Griffin.h"
#include "Ghost.h"
#include "PlayerCharacter.h"
#include "TreasureChest.h"
#include "Treasure.h"
#include "GameTraps.h"
#include "SpikeTrap.h"
#include "ArrowTrap.h"
#include "IdolBoulderTrap.h"
#include "mazeGeneration.h"

class SpawnManager {
public:
	SpawnManager();

	void spawnEnemies(b2World &world,MazeGeneration maze,vector<std::shared_ptr<Skeleton>> &skeletonVector, vector<std::shared_ptr<Medusa>> &medusaVector, vector<std::shared_ptr<Griffin>> &griffinVector, vector<std::shared_ptr<Ghost>> &ghostVector);
	void spawnTreasure(b2World &world,MazeGeneration maze,vector<std::shared_ptr<Treasure>> &treasureVector);
	void spawnChests(b2World &world,MazeGeneration maze,vector<std::shared_ptr<TreasureChest>> &treasureChestVector);
	void spawnTraps(b2World &world, MazeGeneration maze, vector<std::shared_ptr<GameTraps>> &trapsVector);
};