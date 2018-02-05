#pragma once
#include "stdafx.h"

//base class, will be used for inheritance for all the "GameCharacters" so the player and npcs (if we add any) and enemies
class GameCharacters {
public:
	//setup some base variables for gamecharacters that can be inherited 
	int xPosition;
	int yPosition;

	//variables for enemies
	int health;
	int damage;
	float visionRange;
	
	virtual void createSFML() = 0;
	virtual void createCollisionBox(b2World &myWorld) = 0;
	//virtual void update() = 0;

	sf::RectangleShape rectangle;
	sf::IntRect textureSubRect;

	b2BodyDef BodyDef; // the box2d body def. all the classes that inherit form this one are going to nneed bodies so made sense to inherit the bodydef
	b2PolygonShape objectShape; // the box2d shape def. all the classes that inherit form this one are going to nneed shapes so made sense to inherit the shape
	b2FixtureDef objectFixtureDef;// the box2d fixture def. all the classes that inherit form this one are going to nneed fixtures so made sense to inherit the fixture
	b2Body *dynamicBody;

	
	const float scale = 30.f;

};