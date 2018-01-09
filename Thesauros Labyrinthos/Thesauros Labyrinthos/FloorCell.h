#pragma once
#include "Cell.h"

class FloorCell : public Cell {

public:
	FloorCell(int x, int y, sf::Texture &texture);
	void createSFML(int x, int y, sf::Texture &texture) override;
	void update() override;
	


};