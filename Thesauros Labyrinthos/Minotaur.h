#pragma once
#include "GameCharacters.h"
#include "PlayerCharacter.h"
#include "mazeGeneration.h"

class Minotaur : public GameCharacters {
public:
	Minotaur(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(PlayerCharacter *player, MazeGeneration *maze);
	void updatePath(PlayerCharacter * player,MazeGeneration *maze);
	bool dead = false;// is enemy dead? will be changed by the checkDead function
private:
	list<int> path;
	void checkDead();
	sf::Clock pathCheckTimer;
};