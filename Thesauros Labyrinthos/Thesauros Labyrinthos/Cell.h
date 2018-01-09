#pragma once
#include "stdafx.h"
class Cell {
	//virtual // abstarct class that is going to be used by the types of maze cells.
public: 
	virtual void createSFML(int x, int y, sf::Texture &texture) {};
	virtual void update() {};
	sf::RectangleShape rectangle;
};