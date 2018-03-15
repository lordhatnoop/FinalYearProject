#pragma once
#include "stdafx.h"
#include "Treasure.h"
#include "Skeleton.h"
#include "Medusa.h"
#include "Griffin.h"

class DespawnManager {
public:
	DespawnManager();
	vector<std::shared_ptr<Skeleton>>::iterator skeletonIterator;
	vector<std::shared_ptr<Medusa>>::iterator medusaIterator;
	vector<std::shared_ptr<Griffin>>::iterator GriffinIterator;
	vector<Treasure>::iterator treasureIterator;

	void despawnTreasure(vector<Treasure> &treasureVector, b2World &World);
	void despawnSkeleton(vector<std::shared_ptr<Skeleton>> &skeletonVector, b2World &World);
	void despawnMedusa(vector<std::shared_ptr<Medusa>> &medusaVector, b2World &World);
	void desapwnGriffin(vector<std::shared_ptr<Griffin>> &griffinVector, b2World &World);
};
