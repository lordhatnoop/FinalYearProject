#pragma once
#include "CollisionListener.h"
#include "Skeleton.h"
#include "PlayerCharacter.h"


void CollisionListener::BeginContact(b2Contact * contact)
{
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
		static_cast<Skeleton*>(userData2)->health--;// reduce enemy health
	}
	else if (fixture1->GetFilterData().categoryBits == ENEMY && fixture2->GetFilterData().categoryBits == PLAYERPROJECTILE) {
		static_cast<playerArrow*>(userData2)->destroyed = true; //arrow destoryed
		static_cast<Skeleton*>(userData1)->health--;// reduce enemy health
		
	}
}

