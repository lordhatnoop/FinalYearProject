#include "Minotaur.h"

Minotaur::Minotaur(int x, int y)
{
	xPosition = x;
	yPosition = y;
	pathCheckTimer.restart(); //start the timer
	health = 100; //huge amount of health - supposed to be sub boss 
}

void Minotaur::createSFML()
{
}

void Minotaur::createCollisionBox(b2World & myWorld)
{
}

string Minotaur::getName()
{
	return "Minotaur";
}

void Minotaur::update(PlayerCharacter * player,  MazeGeneration *maze)
{
	if (!path.empty()) {

	}
	updatePath(player, maze); //call the path update
	checkDead(); //check if dead
}

void Minotaur::updatePath(PlayerCharacter * player, MazeGeneration *maze)
{
	if(pathCheckTimer.getElapsedTime().asSeconds() > 5){ // limit how frequently we check
		if (player->xPosition != xPosition && player->yPosition != yPosition) { //don't check if already on top of the player to prevent errors
			path = maze->GeneratePath(xPosition / 10, yPosition / 10, player->xPosition / 10, player->yPosition / 10); //keep updating the path to the player
		}
	}
}

void Minotaur::checkDead()
{
	if (health <= 0) {
		dead = true;
	}
}
