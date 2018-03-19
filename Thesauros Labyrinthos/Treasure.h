#pragma once
#include "stdafx.h"
class Treasure {
public:
	Treasure(int x, int y);
	sf::Vector2f position;

	void CreateSFML();
	void CreateBody(b2World &world);
	void collect();
	void decideValue();
	int value;
	bool collected = false;

	b2FixtureDef fixtureDef; // the fixture for collision with the world
	b2FixtureDef sensorFixtureDef;// the fixture that will be a sensor for the player 
	b2BodyDef BodyDef;
	b2PolygonShape Shape;
	b2Body *Body;

	sf::RectangleShape rectangle;

	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007,
		TREASURE = 0x0008
	};
};