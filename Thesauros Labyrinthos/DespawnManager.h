#pragma once
#include "stdafx.h"
#include "Treasure.h"
#include "Skeleton.h"
#include "Medusa.h"
#include "Griffin.h"
#include "Ghost.h"
#include "Cell.h"
#include "TrapHolder.h"
class DespawnManager {
public:
	DespawnManager();
	vector<std::shared_ptr<Cell>>::iterator cellIterator;
	vector<std::shared_ptr<Skeleton>>::iterator skeletonIterator;
	vector<std::shared_ptr<Medusa>>::iterator medusaIterator;
	vector<std::shared_ptr<Griffin>>::iterator GriffinIterator;
	vector<std::shared_ptr<Ghost>>::iterator GhostIterator;
	vector<std::shared_ptr<Treasure>>::iterator treasureIterator;
	vector<std::shared_ptr<GameTraps>>::iterator trapIterator;

	void despawnWalls(vector<std::shared_ptr<Cell>> &cellVector, b2World & World);
	void despawnTreasurevector(vector<std::shared_ptr<Treasure>> &treasureVector, b2World &World);
	void despawnSkeleton(vector<std::shared_ptr<Skeleton>> &skeletonVector, b2World &World);
	void despawnMedusa(vector<std::shared_ptr<Medusa>> &medusaVector, b2World &World);
	void despawnGriffin(vector<std::shared_ptr<Griffin>> &griffinVector, b2World &World);
	void despawnGhost(vector<std::shared_ptr<Ghost>> &ghostVector, b2World &World);
	void despawnTraps(vector<std::shared_ptr<GameTraps>> & trapVector, b2World &World);
};
