#pragma once
#include "cell.h"
#include "stdafx.h"

class FakeWallCell : public Cell {


public:
	FakeWallCell(int x, int y, b2World &world);
	void createSFML() override;
	void update() override;
	void createBox2D(b2World &world);

	int textureCounter;

	

};