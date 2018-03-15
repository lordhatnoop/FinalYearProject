#pragma once
#include "stdafx.h"
#include "GameCharacters.h"
#include "TreasureChest.h"
#include "Treasure.h"
class CollisionListener : public b2ContactListener {

public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void ArrowCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void PlayerRopeCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void EndRopeCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void FlameCloakCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void TreasureChestCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void TreasurePickUP(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);

	//enum that holds the diffrent type of entities for collision filtering
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007,
		TREASURE = 0x0008
	};

private:
	bool contactHandled = false; //used to check if the contact is handled , to prevent checking every combo of collision
};