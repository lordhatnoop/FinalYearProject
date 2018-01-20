#pragma once
#include "Cell.h"

class FloorCell : public Cell {

public:
	FloorCell(int x, int y);
	void createSFML(int x, int y) override;
	void update() override;
	


};