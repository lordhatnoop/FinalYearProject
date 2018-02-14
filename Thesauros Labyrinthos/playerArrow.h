#pragma once
#include "stdafx.h"
class playerArrow {
public:

	playerArrow(int x, int y, bool playerDirection);
	void createSFML();
	void createHitBox(b2World &myWorld);

	//will store the info passed on creation
	int xPosition;
	int yPosition;
	bool direction; //store the player direction and use it for our direction. still right = true and left = false

	//sfml stuff
	sf::RectangleShape arrowRect;
	sf::IntRect arrowTextureRect;

	//BOx2d stuff

	b2BodyDef BodyDef; // the box2d body def. 
	b2PolygonShape Shape; // the box2d shape def. 
	b2FixtureDef FixtureDef;// the box2d fixture def. 
	
	b2Body *dynamicBody;


	//share the categories so that can filter collision.
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005
	};
};