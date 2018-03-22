#pragma once
#include "GameCharacters.h"
#include "PlayerCharacter.h"
#include "TextureLoader.h"

class Griffin : public GameCharacters {
public:

	Griffin(int x, int y);

	void createSFML();
	void createCollisionBox(b2World &myWorld);

	void update(PlayerCharacter *player);
	void LookForPlayer(PlayerCharacter *player);

	void Charge();
	void Wander();

	void checkDead();

	sf::Vector2f ChargeStartPosition;
	sf::Vector2f wanderStartPosition;
	bool chargeStart = false;
	bool wanderStart = false;
	int chargeDistance = 60;
	int wanderDistance;
	int animationCounter = 0;
	sf::Clock timer;
	sf::Clock wanderTimer;
	int leftORRight;
	string getName() { return "Griffin"; };
	bool dead = false;
};