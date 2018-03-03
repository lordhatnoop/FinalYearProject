#pragma once
#include "GameCharacters.h"
#include "TorchLight.h"
#include "playerArrow.h"
class PlayerCharacter : public GameCharacters {

public:
	PlayerCharacter(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(float dt, b2World &myWorld);
	TorchLight* torch;

	int treasure = 0;//hold the amount of treasure

	int playerHealth = 2; // player health, starts at 2 and can be upgraded

	float maxTorchFuel = 100.f; // will be upgradable, holds max fuel
	float currentTorchFuel = 100.f;//set this to equal maxTorchfuel at the start to ensure max fuel at the beggining of the game

	bool m_contactingEnemy = false;

	bool playerInvincible = false; // bool for check if the player is invincible, used for when been hit to give invincibility for a few seconds, can also be used for items

	bool canJump = false; //check for if we can jump

	bool facingLeftORRight = true; // true = right, false = left;

	bool isStone = false; //bool used to check if we have been turned to stone by a medusa enemy

	b2FixtureDef footSensor;
	b2FixtureDef leftSensor;
	b2FixtureDef rightSensor;

	vector<std::shared_ptr<playerArrow>> arrowVector;
private:
	void torchCountdown();
	//update clocks
	sf::Clock *updateclock;
	sf::Clock *invincibilityClock;

	void collidingWithEnemy();

	//function for invincibility
	void InvincibleTimer();

	//walk animation counter, used to change which frame of animation to display
	int walkAnimationCounter = 1;
	void walkRight();
	void walkLeft();
	
	//bool Box2dCreated = false;
};