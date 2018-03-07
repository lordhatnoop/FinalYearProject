#pragma once
#include "stdafx.h"
#include "GameCharacters.h"
class CollisionListener : public b2ContactListener {

public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void ArrowCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	void PlayerRopeCollision(void* userData1, void* userData2, b2Fixture* fixture1, b2Fixture* fixture2);
	//enum that holds the diffrent type of entities for collision filtering
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006
	};

};