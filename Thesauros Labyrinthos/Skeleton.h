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
	bool dead = false;// is enemy dead? will be changed by the checkDead function
private:
	bool risen = false; // bool to check if done the rise aniuamtion 
	bool triggerRise = false; // bool to trigger the rise animation

	
	void rise(); //function to do rise animation

	int walkAnimationFrame = 0;
	void moveRight();
	void moveLeft();
	void attack();
	void checkDead();
};

