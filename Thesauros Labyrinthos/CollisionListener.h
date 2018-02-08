#pragma once
#include "stdafx.h"
#include "GameCharacters.h"
class CollisionListener : public b2ContactListener {

public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	//enum that holds the diffrent type of entities for collision filtering
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,

	};

};