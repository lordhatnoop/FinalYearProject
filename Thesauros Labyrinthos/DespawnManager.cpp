#include "DespawnManager.h"

DespawnManager::DespawnManager()
{
}

void DespawnManager::despawnTreasure(vector<Treasure> &treasureVector, b2World & World)
{
	for (treasureIterator = treasureVector.begin(); treasureIterator != treasureVector.end();) {
		if (treasureIterator->collected == true) { //if treasure collected
			World.DestroyBody(treasureIterator->Body); //destroy the body
			treasureIterator = treasureVector.erase(treasureIterator); // remove from treasure vector
		}
		else {
			treasureIterator++;
		}
	}
}

void DespawnManager::despawnSkeleton(vector<std::shared_ptr<Skeleton>> &skeletonVector, b2World & World)
{
	for (skeletonIterator = skeletonVector.begin(); skeletonIterator != skeletonVector.end();) {
		if ((*skeletonIterator)->dead == true) { //if skeleton destroyed
			World.DestroyBody((*skeletonIterator)->dynamicBody); //destroy the body
			skeletonIterator = skeletonVector.erase(skeletonIterator); // remove from skeleton vector
		}
		else {
			skeletonIterator++;
		}
	}
}

void DespawnManager::despawnMedusa(vector<std::shared_ptr<Medusa>> &medusaVector, b2World & World)
{
	for (medusaIterator = medusaVector.begin(); medusaIterator != medusaVector.end();) {
		if ((*medusaIterator)->dead == true) { //if medusa destroyed
			World.DestroyBody((*medusaIterator)->dynamicBody); //destroy the body
			medusaIterator = medusaVector.erase(medusaIterator); // remove from  vector
		}
		else {
			medusaIterator++;
		}
	}
}

void DespawnManager::desapwnGriffin(vector<std::shared_ptr<Griffin>> &griffinVector, b2World & World)
{
	for (GriffinIterator = griffinVector.begin(); GriffinIterator != griffinVector.end();) {
		if ((*GriffinIterator)->dead == true) { //if griffin destroyed
			World.DestroyBody((*GriffinIterator)->dynamicBody); //destroy the body
			GriffinIterator = griffinVector.erase(GriffinIterator); // remove from  vector
		}
		else {
			GriffinIterator++;
		}
	}
}
