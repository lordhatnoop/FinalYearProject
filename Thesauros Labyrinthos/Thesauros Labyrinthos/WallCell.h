#pragma once
#include "Cell.h"

class WallCell : public Cell {


public:
	WallCell(int x , int y, sf::Texture &texture);
	void createSFML(int x, int y, sf::Texture &texture) override ;
	void update() override;
	void createCollisionBox();


};