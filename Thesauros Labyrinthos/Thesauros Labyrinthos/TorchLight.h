#pragma once
#include "stdafx.h"
class TorchLight
{
public:
	TorchLight(int playerX, int playerY);
	void torchUpdate(float maxFuel, float currentFuel);// will handle dimming/ brightening the light based on fuel
	void update(int playerX, int playerY,float maxFuel, float currentFuel); //two updates because i wanted to spliut the update code a bit. this one will handle updating the position to match the player's and then call the torchUpdate
	sf::Sprite torchSprite; // will hold the end result so it can be drawn
private:
	int xPos;
	int yPos;
	sf::RenderTexture torchTexture; //used to hold and combine the elements for the torchsprite
	sf::CircleShape tempLightCircle; // create a circleshape
};