#pragma once
#include "GameCharacters.h"
#include "PlayerCharacter.h"
class Medusa : public GameCharacters
{
public:
	~Medusa();

	Medusa(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(PlayerCharacter *player);
	void LookForPlayer(PlayerCharacter *player);

	sf::Clock timer;
	sf::Clock petrifyTimer;
	bool dead = false;// is enemy dead? will be changed by the checkDead function

private:
	

	bool playerInAttackRange = false;
	int walkAnimationFrame = 0;
	int attackAnimationFrame = 0;
	void moveRight();
	void moveLeft();
	void attack(PlayerCharacter *player);
	void checkDead();

};