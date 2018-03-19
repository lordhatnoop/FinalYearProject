#pragma once
#include "stdafx.h"
#include "Treasure.h"
#include "Skeleton.h"
#include "Medusa.h"
#include "Griffin.h"
#include "Ghost.h"

class DespawnManager {
public:
	DespawnManager();
	vector<std::shared_ptr<Skeleton>>::iterator skeletonIterator;
	vector<std::shared_ptr<Medusa>>::iterator medusaIterator;
	vector<std::shared_ptr<Griffin>>::iterator GriffinIterator;
	vector<std::shared_ptr<Ghost>>::iterator GhostIterator;
	vector<Treasure>::iterator treasureIterator;

	void despawnTreasure(vector<Treasure> &treasureVector, b2World &World);
	void despawnSkeleton(vector<std::shared_ptr<Skeleton>> &skeletonVector, b2World &World);
	void despawnMedusa(vector<std::shared_ptr<Medusa>> &medusaVector, b2World &World);
	void despawnGriffin(vector<std::shared_ptr<Griffin>> &griffinVector, b2World &World);
	void despawnGhost(vector<std::shared_ptr<Ghost>> &ghostVector, b2World &World);
};
