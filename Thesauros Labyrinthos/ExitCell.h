#pragma once

#include "Cell.h"
#include "PlayerCharacter.h"
class ExitCell : public Cell
{
public:
	
	ExitCell(int x, int y);

	// the position
	int posX;
	int posY;

	//bool for whether level reached. ccontrols switching levels
	bool ExitReached = false;

	void update();

private:
	void createSFML();
	
	//use to check if the player has reached the exit
	void playerReached(PlayerCharacter player);
};