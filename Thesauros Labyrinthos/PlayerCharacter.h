#pragma once
#include "GameCharacters.h"
#include "TorchLight.h"
#include "playerArrow.h"
#include "GameItems.h"
#include "RopeItem.h"
class PlayerCharacter : public GameCharacters {

public:
	PlayerCharacter(int x, int y);
	void createSFML();
	void createCollisionBox(b2World &myWorld);
	string getName();
	void update(float dt, b2World &myWorld);
	TorchLight* torch;
	b2CircleShape circleShape;
	int treasure = 0;//hold the amount of treasure

	int playerHealth = 3; // player health, starts at 2 and can be upgraded
	int playerMaxHealth = 3; // the maxiumum amount of health the player can currently have. can also be upgraded, the upgrades for max and current health are seperate, with current health upgrade being a health refill
	
	float maxTorchFuel = 100.f; // will be upgradable, holds max fuel
	float currentTorchFuel = 100.f;//set this to equal maxTorchfuel at the start to ensure max fuel at the beggining of the game

	float ItemDamageMultiplier = 1.f; // a multiplier that will be sued to increase item damage - this is what gets upgraded to improve item damage
	//bools used for various player and item update things
	bool m_contactingEnemy = false;

	bool playerInvincible = false; // bool for check if the player is invincible, used for when been hit to give invincibility for a few seconds, can also be used for items

	bool canJump = false; //check for if we can jump

	bool facingLeftORRight = true; // true = right, false = left;

	bool isStone = false; //bool used to check if we have been turned to stone by a medusa enemy

	bool canClimb = false; //bool used to control when the player can use the W key to climb. will be activated when touching a rope. do this so that player can only climb when touching the sensor body of the rope

	PlayerItemStatuses itemStatuses; //used to hold , bools,etc related to items for convenience

	//three sensors used for jumping and wall climbing
	b2FixtureDef footSensor;
	b2FixtureDef leftSensor;
	b2FixtureDef rightSensor;

	//vector to store created arrows
	vector<std::shared_ptr<playerArrow>> arrowVector;

	//ITEMS WOO
	vector<std::shared_ptr<GameItems>> AquiredItems;
	std::shared_ptr<GameItems> activeItem; // the current active item
	int currentItem = 0;
	bool itemsOnCooldown = false;
	bool updateCurrentItem;
	sf::Clock *CurrentItemClock;
	b2Vec2 temp;

	//clock for us being petrified
	sf::Clock *petrifiedClock;
	sf::RectangleShape petrifyOverlay; //pertriefied overlay

	//shield variables
	float shieldEnergy = 100.f;
	float shieldEnergyMax = 100.f;
	bool shielding = false;
	bool playerHit = false;
	sf::CircleShape shieldCircle;

private:
	void torchCountdown();
	//update clocks
	sf::Clock *updateclock;
	sf::Clock *invincibilityClock;

	//called if the bool for colliding is set to true by the collision listener
	void collidingWithEnemy();

	//method for using the shield
	void playerShield();
	

	//function for invincibility
	void InvincibleTimer();

	//walk animation counter, used to change which frame of animation to display
	int walkAnimationCounter = 1;
	void walkRight();
	void walkLeft();
	sf::Clock animationTimer;

	//action to take when petrified (by medusa setting (is stone) to true)
	void Petrified();
	

	void ItemUse(b2World &myWorld);
	//bool Box2dCreated = false;
};