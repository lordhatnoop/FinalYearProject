#pragma once

#include "Cell.h"

class WallCell : public Cell {


public:
	WallCell(int x , int y, b2World &world);
	void createSFML() override ;
	void update() override;
	void createBox2D(b2World &world);
	 
	int textureCounter;
	
	b2Body *staticBody;

};