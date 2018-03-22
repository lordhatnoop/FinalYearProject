#pragma once
#include "CollisionListener.h"
#include "Skeleton.h"
#include "PlayerCharacter.h"
#include "GameCharacters.h"
#include "Cell.h"


void CollisionListener::BeginContact(b2Contact * contact)
{
	contactHandled = false; //reset the bool
	//get the fixtures
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	//get the userdata for both 
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

	/*
	if (bodyUserData == nullptr || bodyUserData2 == nullptr) {
		return;
	}
	*/

	//this method works for all objects, but categories could get quite long, and it requires the enum for the categories to be declared in each class that needs a category
	//if the two fixture's categories are player and enemy
	if (fixtureA->GetFilterData().categoryBits == PLAYER && fixtureB->GetFilterData().categoryBits == ENEMY) {
		static_cast<PlayerCharacter*>(bodyUserData)->m_contactingEnemy = true; // get the player that is being collided with, and set it's bool for colliding with enemies to true
	}
	else if (fixtureA->GetFilterData().categoryBits == ENEMY && fixtureB->GetFilterData().categoryBits == PLAYER) {
		static_cast<PlayerCharacter*>(bodyUserData2)->m_contactingEnemy = true;// get the player that is being collided with, and set it's bool for colliding with enemies to true
	}

	//check if player is grounded, or touching another wall
	else if (fixtureA->GetFilterData().categoryBits == PLAYER && fixtureB->GetFilterData().categoryBits == WALL) {
		if (fixtureA->IsSensor()) { // make sure it's a sensor. sensors are on the bottom and left/ right of player. check this so that we can only jump if bottom or left / right are touching and not the top of the player
			//only sensors are the ones being used for this so don't need to check data or anything
			static_cast<PlayerCharacter*>(bodyUserData)->canJump = true; // get the player that is being collided with, and set it's bool for being able to jump to true
		}
	}
	else if (fixtureA->GetFilterData().categoryBits == WALL && fixtureB->GetFilterData().categoryBits == PLAYER) {
		if (fixtureB->IsSensor()) { // make sure player fixture is sensor
			static_cast<PlayerCharacter*>(bodyUserData2)->canJump = true; // get the player that is being collided with, and set it's bool for being able to jump to true
		}
	}

	ArrowCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for arrow collision
	PlayerRopeCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for player and rope collision
	FlameCloakCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for FlameCloakCollision
	TreasureChestCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for treasureChest collision
	TreasurePickUP(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for treasurePickUp
	BombExplosion(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for bomb explosion hitting walls
	MedusaHeadPetrify(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for MedusaHEad body collision and turn enemies to stone
	SpikeTrapCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for player hitting spikes
	ArrowTrapCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for Arrow Trap collision
	GoldenIdolCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB); //check for golden Idol Trap collision
	/*
	//get what objects each one is 
	std::string nameA = ((GameCharacters*)bodyUserData)->getName();
	std::string nameB = ((GameCharacters*)bodyUserData2)->getName();

	//if the first name is playerCharacter and the second has the first 5 characters as ENemy
	if (nameA == "PlayerCharacter" &&  nameB.compare(0, 5, "Enemy") == 0) {
		static_cast<PlayerCharacter*>(bodyUserData)->m_contactingEnemy = true;
	}
	else if (nameA.compare(0, 5, "Enemy") == 0 && nameB == "PlayerCharacter") {
		static_cast<PlayerCharacter*>(bodyUserData2)->m_contactingEnemy = true;
	}
	*/ //works for just enemies


}
void CollisionListener::EndContact(b2Contact * contact)
{
	contactHandled = false; //reset the contactHandled bool to be used again
	//get the fixtures
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	//get the userdata for both 
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserData2 = contact->GetFixtureB()->GetBody()->GetUserData();

	//cleanup for touching walls for jumping
	//check if for player and wall
	if (fixtureA->GetFilterData().categoryBits == PLAYER && fixtureB->GetFilterData().categoryBits == WALL) {
		static_cast<PlayerCharacter*>(bodyUserData)->canJump = false; // set back to false, becasue collision is ending
	}
	else if (fixtureA->GetFilterData().categoryBits == WALL && fixtureB->GetFilterData().categoryBits == PLAYER) {
		static_cast<PlayerCharacter*>(bodyUserData2)->canJump = false; // set back to false, becasue collision is ending
	}

	//put every subsequent method call in an else if checking the bool contacthandled  is false so that it only runs until the collision is handled
	else if (contactHandled == false) {
		//run the the end code for rope collision to check if the collision ending is for the player and rope
		EndRopeCollision(bodyUserData, bodyUserData2, fixtureA, fixtureB);
	}
	
}

//listen for arrow collisions and react
void CollisionListener::ArrowCollision(void * userData1, void * userData2, b2Fixture* fixture1, b2Fixture* fixture2)
{
	//if arrow and wall collision
	//wall wilkl be fine, destroy arrow
	if (fixture1->GetFilterData().categoryBits == PLAYERPROJECTILE && fixture2->GetFilterData().categoryBits == WALL) {
		static_cast<playerArrow*>(userData1)->destroyed = true; //set arrow to be destroyed
	}
	else if (fixture1->GetFilterData().categoryBits == WALL && fixture2->GetFilterData().categoryBits == PLAYERPROJECTILE) {
		static_cast<playerArrow*>(userData2)->destroyed = true;
	}

	//arrow enemy collision
	else if (fixture1->GetFilterData().categoryBits == PLAYERPROJECTILE && fixture2->GetFilterData().categoryBits == ENEMY) {
		static_cast<playerArrow*>(userData1)->destroyed = true; //arrow destoryed
		static_cast<GameCharacters*>(userData2)->health--;// reduce enemy health
		if (static_cast<GameCharacters*>(userData2)->playerLeft == true) {
			static_cast<GameCharacters*>(userData2)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(1, 0), true);
		}
		else {
			static_cast<GameCharacters*>(userData2)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(-1, 0), true);
		}
	}
	else if (fixture1->GetFilterData().categoryBits == ENEMY && fixture2->GetFilterData().categoryBits == PLAYERPROJECTILE) {
		static_cast<playerArrow*>(userData2)->destroyed = true; //arrow destoryed
		static_cast<GameCharacters*>(userData1)->health--;// reduce enemy health
		if (static_cast<GameCharacters*>(userData1)->playerLeft == true) {
			static_cast<GameCharacters*>(userData1)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(1, 0), true);
		}
		else {
			static_cast<GameCharacters*>(userData1)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(-1, 0), true);
		}
	}
}

void CollisionListener::PlayerRopeCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == PLAYER) {
		if (userData1 == "Rope") { //check the userdata of the second colliding item for what type of item it is.
			static_cast<PlayerCharacter*>(userData2)->canClimb = true; //set the player can climb to true so that they can climb the rope
		}
	}
	else if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "Rope") {
			static_cast<PlayerCharacter*>(userData1)->canClimb = true;
		}
	}
}

// the function to run once the collison with a rope has ended
void CollisionListener::EndRopeCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == PLAYER) {
		if (userData1 == "Rope") { //check the userdata of the second colliding item for what type of item it is.
			static_cast<PlayerCharacter*>(userData2)->canClimb = false; //set the player can climb to false so that they can no longer climb the rope
		}
	}
	else if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "Rope") {
			static_cast<PlayerCharacter*>(userData1)->canClimb = false;
		}
	}
}

//check for flameCloak collision with enemies
void CollisionListener::FlameCloakCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == ENEMY) {
		if (userData1 == "FlameCloak") { //check the userdata of the second colliding item for what type of item it is.
			static_cast<GameCharacters*>(userData2)->health = static_cast<GameCharacters*>(userData1)->health - static_cast<GameItems*>(userData1)->itemDamage; // deal the item damage to the enemy
			
			//check which side the player is on to check which way to knock the enemy back, then apply the knockbnack force
			if (static_cast<GameCharacters*>(userData2)->playerLeft = true) {
				static_cast<GameCharacters*>(userData2)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(0.25f, 0.f), true);
			}
			else if (static_cast<GameCharacters*>(userData2)->playerRight = true) {
				static_cast<GameCharacters*>(userData2)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(-0.25f, 0.f), true);
			}
		}
	}
	else if (fixture1->GetFilterData().categoryBits == ENEMY && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "FlameCloak") {
			static_cast<GameCharacters*>(userData1)->health = static_cast<GameCharacters*>(userData1)->health - static_cast<GameItems*>(userData2)->itemDamage;
		
			//check which side the player is on to check which way to knock the enemy back, then apply the knockbnack force
			if (static_cast<GameCharacters*>(userData1)->playerLeft = true) {
				static_cast<GameCharacters*>(userData1)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(0.25f, 0.f), true);
			}
			else if (static_cast<GameCharacters*>(userData1)->playerRight = true) {
				static_cast<GameCharacters*>(userData1)->dynamicBody->ApplyLinearImpulseToCenter(b2Vec2(-0.25f, 0.f), true);
			}
		}
	}
}

void CollisionListener::TreasureChestCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == TREASURECHEST && fixture2->GetFilterData().categoryBits == PLAYER) { // if it's the correct type of collision
		static_cast<TreasureChest*>(userData1)->openChest(static_cast<PlayerCharacter*>(userData2)); //if the player touches the treasure chest, open the chest
	}
	else if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == TREASURECHEST) {
		static_cast<TreasureChest*>(userData2)->openChest(static_cast<PlayerCharacter*>(userData1)); //if the player touches the treasure chest, open the chest
	}
}

void CollisionListener::TreasurePickUP(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == TREASURE && fixture2->GetFilterData().categoryBits == PLAYER) {
		//add the value to the player's treasure and set the treasure collected
		static_cast<Treasure*>(userData1)->collect();
		static_cast<PlayerCharacter*>(userData2)->treasure = static_cast<PlayerCharacter*>(userData2)->treasure + static_cast<Treasure*>(userData1)->value;
		
	}
	else if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == TREASURE) {
		static_cast<PlayerCharacter*>(userData1)->treasure = static_cast<PlayerCharacter*>(userData1)->treasure + static_cast<Treasure*>(userData2)->value;
		static_cast<Treasure*>(userData2)->collect();
	}
}

void CollisionListener::BombExplosion(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if(fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == WALL) {
		//check if bomb and fake wall, then destroy if it is
		if (userData1 == "BombItem") { // make sure it was a bomb
			if (static_cast<Cell*>(userData2)->cellType == "Fake wall cell") { // make sure it was fake wall cell
				static_cast<Cell*>(userData2)->destroyed = true; //set to be destroyed
			}
			
		}
	}
	else if (fixture1->GetFilterData().categoryBits == WALL && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "BombItem") { // make sure it was a bomb
			if (static_cast<Cell*>(userData1)->cellType == "Fake wall cell") { // make sure it was fake wall cell
				static_cast<Cell*>(userData1)->destroyed = true; //set to be destroyed
			}
		}
	}
	 // now check for enemies
	else if (fixture1->GetFilterData().categoryBits == ENEMY && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "BombItem") { //check if bomb hit enemies and minus the damage if they did
			static_cast<GameCharacters*>(userData1)->health = static_cast<GameCharacters*>(userData1)->health - static_cast<GameItems*>(userData2)->itemDamage;
		}
	}
	else if (fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == ENEMY) {
		if (userData1 == "BombItem") { //check if bomb hit enemies and minus the damage if they did
			static_cast<GameCharacters*>(userData2)->health = static_cast<GameCharacters*>(userData2)->health - static_cast<GameItems*>(userData1)->itemDamage;
		}
	}
}


//check for collision between the enemies and the medusa head item
void CollisionListener::MedusaHeadPetrify(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == ITEM && fixture2->GetFilterData().categoryBits == ENEMY) {
		//check if actually medusaHead and turn to stone
		if (userData1 == "MedusaHead") { // make sure it was the medusaHead body
			
				static_cast<GameCharacters*>(userData2)->isStone = true; //set to be stone
			

		}
	}
	else if (fixture1->GetFilterData().categoryBits == ENEMY && fixture2->GetFilterData().categoryBits == ITEM) {
		if (userData2 == "MedusaHead") { // make sure it was a the medusaHead
		
				static_cast<GameCharacters*>(userData1)->isStone = true; //set to be Stone
			
		}
	}
}

void CollisionListener::SpikeTrapCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == TRAPS) {
		//check if actually spiketrap and kill if so
		if (userData2 == "SpikeTrap") { // make sure it was the spiketrap
			if (fixture1->GetUserData() == "FootSensor") { //make sure it was the foot sensor colliding
				static_cast<PlayerCharacter*>(userData1)->playerHealth = 0; //set to be dead
			}

		}
	}
	else if (fixture1->GetFilterData().categoryBits == TRAPS && fixture2->GetFilterData().categoryBits == PLAYER) {
 		if (userData1 == "SpikeTrap") { // make sure it was a the spike trap
			if (fixture2->GetUserData() == "FootSensor") { //make sure it was the foot sensor colliding
				static_cast<PlayerCharacter*>(userData2)->playerHealth = 0; //set to be dead 
			}
		}
	}
}

void CollisionListener::ArrowTrapCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == TRAPS) {
		//check if actually spiketrap and kill if so
		if (userData2 == "ArrowTrap") { // make sure it was the spiketrap
			if (fixture2->GetUserData() == "Arrow") {
				static_cast<PlayerCharacter*>(userData1)->playerHealth--; //minus health if it was the arrow section of the trap colliding
			}
			else {
				static_cast<GameTraps*>(fixture2->GetUserData())->setTriggered(); //trigger the trap
			}
		}
	}
	else if (fixture1->GetFilterData().categoryBits == TRAPS && fixture2->GetFilterData().categoryBits == PLAYER) {
		if (userData1 == "ArrowTrap") { // make sure it was a the spike trap
			if (fixture1->GetUserData() == "Arrow") { // if arrow section of the trap
				static_cast<PlayerCharacter*>(userData2)->playerHealth--; //minus health if it was the arrow section of the trap colliding
			}
			else { //else
				static_cast<GameTraps*>(fixture1->GetUserData())->setTriggered(); //trigger the trap
			}
		}
	}
}

void CollisionListener::GoldenIdolCollision(void * userData1, void * userData2, b2Fixture * fixture1, b2Fixture * fixture2)
{
	if (fixture1->GetFilterData().categoryBits == PLAYER && fixture2->GetFilterData().categoryBits == TRAPS) {
		//check if actually spiketrap and kill if so
		if (userData2 == "Idol") { // make sure it was the Idol
			if (fixture2->GetUserData() == "Boulder") {
				static_cast<PlayerCharacter*>(userData1)->playerHealth = 0; //player crushed and dead
			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //allow player to pick it up with the arrow key
					static_cast<GameTraps*>(fixture2->GetUserData())->setTriggered(); //trigger the trap
					static_cast<GameTraps*>(fixture2->GetUserData())->TrapDelayTimer.restart(); //start the delay timer
					static_cast<PlayerCharacter*>(userData1)->treasure = static_cast<PlayerCharacter*>(userData1)->treasure + 1000; //add treasure on the pickup
				}
			}
		}
	}
	else if (fixture1->GetFilterData().categoryBits == TRAPS && fixture2->GetFilterData().categoryBits == PLAYER) {
		if (userData1 == "Idol") { // make sure it was a Idol trap
			if (fixture1->GetUserData() == "Boulder") { // if Boulder section of the trap
				
				static_cast<PlayerCharacter*>(userData2)->playerHealth = 0; // player crushed and dead
			}
			else { //else
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //allow player to pick it up with the arrow key
					static_cast<GameTraps*>(fixture1->GetUserData())->setTriggered(); //trigger the trap
					static_cast<GameTraps*>(fixture1->GetUserData())->TrapDelayTimer.restart(); //start the delay timer
					static_cast<PlayerCharacter*>(userData2)->treasure = static_cast<PlayerCharacter*>(userData2)->treasure + 1000; //add treasure on the pickup
				
				}
			}
		}
	}
}



