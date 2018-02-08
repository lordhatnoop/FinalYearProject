#pragma once
#include "GameCharacters.h"
#include "PlayerCharacter.h"
class Skeleton : public GameCharacters
{
public:
	
	~Skeleton();

	Skeleton(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(PlayerCharacter *player);
	void LookForPlayer(PlayerCharacter *player);
	sf::Clock timer;
private:
	bool risen = false; // bool to check if done the rise aniuamtion 
	bool triggerRise = false; // bool to trigger the rise animation

	//bools for which side the player is on
	bool playerLeft = false;
	bool playerRight = false; 

	void rise(); //function to do rise animation

	int walkAnimationFrame = 0;
	void moveRight();
	void moveLeft();
	void attack();
};

