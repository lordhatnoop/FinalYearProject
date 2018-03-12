#pragma once
#include "stdafx.h"



//base class, will be used for inheritance for all the "GameCharacters" so the player and npcs (if we add any) and enemies
class GameCharacters {

	
public:
	//setup some base variables for gamecharacters that can be inherited 
	int xPosition;
	int yPosition;

	//variables for enemies
	float health;
	int damage;
	float visionRange;
	
	virtual void createSFML() = 0;
	virtual void createCollisionBox(b2World &myWorld) = 0;
	//virtual void update() = 0;

	virtual string getName() = 0;


	sf::RectangleShape rectangle;
	sf::IntRect textureSubRect;

	b2BodyDef BodyDef; // the box2d body def. all the classes that inherit form this one are going to nneed bodies so made sense to inherit the bodydef
	b2PolygonShape Shape; // the box2d shape def. all the classes that inherit form this one are going to nneed shapes so made sense to inherit the shape
	b2FixtureDef FixtureDef;// the box2d fixture def. all the classes that inherit form this one are going to nneed fixtures so made sense to inherit the fixture
	b2FixtureDef sensorFixture; // a second fixture to be used as a sensor
	b2Body *dynamicBody;
	
	bool m_contactingPlayer = false;
	
	const float scale = 30.f;

	//bools for which side the player is on
	bool playerLeft = false;
	bool playerRight = false;

	sf::Sound sound;
	//enum that holds the diffrent type of entities for collision filtering
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007
	};

};