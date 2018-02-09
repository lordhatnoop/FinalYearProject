#pragma once
class playerArrow {
public:

	playerArrow(int x, int y, bool playerDirection);
	void createSFML();
	void createHitBox();

	//will store the info passed on creation
	int xPosition;
	int yPosition;
	bool direction; //store the player direction and use it for our direction. still right = true and left = false
};