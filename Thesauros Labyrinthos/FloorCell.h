#pragma once

#include "Cell.h"

class FloorCell : public Cell {

public:
	FloorCell(int x, int y);
	void createSFML() override;
	
	void update() override;
	


};