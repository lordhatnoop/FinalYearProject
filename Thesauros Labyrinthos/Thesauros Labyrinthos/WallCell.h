#pragma once
#include "Cell.h"

class WallCell : public Cell {


public:
	WallCell(int x , int y);
	void createSFML(int x, int y) override ;
	void update() override;
	void createCollisionBox();
	
	int textureCounter;
};