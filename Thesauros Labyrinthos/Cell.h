#pragma once
#include "stdafx.h"
#include "GameCharacters.h"


class Cell {
	//virtual // abstarct class that is going to be used by the types of maze cells.
public: 
	
	virtual void update() {};
	sf::RectangleShape rectangle;
	string cellType; // sued for idnetifying cells in the vector
	bool ExitReached = false;
	b2Body *cellBody;
	bool destroyed = false;
protected:
	const int scale = 30.f; //scale used for box2d - 30 pixels to a meter, used in multiplication and dividing to get appropriate physics for while keeping same pixel size.
	sf::Vector2f cellSize; // vector2f to store the objectsize
	sf::Vector2f cellPosition; // vector 2f to store the objectPosition

	//setup the bodydef, shape and fixture def for box2d
	b2BodyDef BodyDef;
	b2PolygonShape cellBox2DPolygonShape;
	
	b2FixtureDef objectFixtureDef;



	//enum that holds the diffrent type of entities for collision filtering
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


private:
	virtual void createSFML() {};
	virtual void createBox2D(b2World &world) {};
};