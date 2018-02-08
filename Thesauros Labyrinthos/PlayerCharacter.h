#pragma once
#include "GameCharacters.h"
#include "TorchLight.h"
class PlayerCharacter : public GameCharacters {

public:
	PlayerCharacter(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(float dt);
	TorchLight* torch;

	int treasure = 0;//hold the amount of treasure

	int playerHealth = 2; // player health, starts at 2 and can be upgraded

	float maxTorchFuel = 100.f; // will be upgradable, holds max fuel
	float currentTorchFuel = 100.f;//set this to equal maxTorchfuel at the start to ensure max fuel at the beggining of the game

	bool m_contactingEnemy = false;

	bool playerInvincible = false; // bool for check if the player is invincible, used for when been hit to give invincibility for a few seconds, can also be used for items

	bool canJump = false; //check for if we can jump

private:
	void torchCountdown();
	sf::Clock *updateclock;
	sf::Clock *invincibilityClock;
	void collidingWithEnemy();
	void InvincibleTimer();
	bool Box2dCreated = false;
};