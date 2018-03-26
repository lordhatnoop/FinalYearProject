#pragma once
#include "TextureLoader.h"
#include "stdafx.h"
class GameTraps {

public:

	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007,
		TREASURE = 0x0008,
		TRAPS = 0x0009
	};

	//provide two fixturedefs by standard
	b2FixtureDef fixture1; 
	b2FixtureDef fixture2;
	b2Vec2 temp;
	b2BodyDef BodyDef;
	b2PolygonShape Shape;
	b2Body *Box2DBody;

	sf::RectangleShape rectangle;
	sf::IntRect textureSubRect;

	float xPosition;
	float yPosition;
	int scale = 30;
	string trapName;
	bool trapTriggered = false;
	sf::Clock TrapDelayTimer;
	virtual void update() {};
	virtual void createSfml() {};
	virtual void createBox2D(b2World &myWorld) {};
	virtual void setTriggered() {};
	bool deactivated = false;
};