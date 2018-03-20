#pragma once
#include "stdafx.h"
#include "PlayerItemStatuses.h"

class GameItems {
public:
	//have the item categories, cause i'm going to want to detect collisions
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007,
		TREASURE = 0x0008
	};
	
	//want the items to have two fixtures, one for colliding with walls, etc and proper collision so that the items don't fall through the world
	// and a second that detcts player collision while letting the player walk through it, so a sensor so we detect the collision but don't have any forces
	// these aren't set like that here so that we can chnage this if needed in the individual item classes that inherit from this
	b2FixtureDef ItemWorldFixture; // the fixture for collision with the world
	b2FixtureDef ItemPlayerSensorFixture;// the fixture that will be a sensor for the player 
	b2Vec2 temp;
	b2BodyDef BodyDef; 
	b2PolygonShape Shape; 
	b2Body *Box2DBody;

	sf::RectangleShape rectangle;
	sf::IntRect textureSubRect;
	sf::Clock *durationTimer;
	float xPosition;
	float yPosition;
	int coolDownTimer;
	int duration;
	bool active = false;
	int scale = 30;
	float itemDamage;
	string itemName;
	bool sprintActive = false;
	virtual void update(PlayerItemStatuses *playerStatus) {};
	virtual void createSfml() {};
	virtual void createBox2D(b2World &myWorld) {};
	virtual void destroy(b2World & myWorld) {};
};