#pragma once
#include "Cell.h"
class NonBodyWallCell : public Cell
{
public:

	
	void createSFML() override;

	NonBodyWallCell(int x, int y);
	~NonBodyWallCell();
};

