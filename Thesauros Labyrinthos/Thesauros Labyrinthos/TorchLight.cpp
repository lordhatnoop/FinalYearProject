#include "TorchLight.h"


TorchLight::TorchLight(int playerX, int playerY)
{
	//get the inital position 
	xPos = playerX;
	yPos = playerY;

	torchTexture.create(1600, 900);// create the renderTexture and set it to be the size of the window
	torchTexture.clear(sf::Color(0, 0, 0, 250));// clear the rendertexture and set it to be black with a slight transparancy
	 
	tempLightCircle.setFillColor(sf::Color(0, 0, 0, 150)); // set the colour of the circle to be black. due to the blending we're going to do, this black and the black background
	//will cancel each other out. The lower the alpha, the bright this circle will appear
	tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 170)); // same as above but slightly darker to represent darkening edges
	tempLightCircle.setOutlineThickness(5);
	tempLightCircle.setRadius(40); // set the radius
	tempLightCircle.setPosition(xPos, yPos); // set the position of the light circle. Will be the playerPosition
	tempLightCircle.setOrigin(tempLightCircle.getRadius(), tempLightCircle.getRadius());


	torchTexture.draw(tempLightCircle, sf::BlendMultiply); // get the renderTexture to draw the circle, the blend mode will cause the two black areas to cancel out and cause a light spot
	torchTexture.display();// tell the renderTexture to display to finalise the texture it will give

	torchSprite.setTexture(torchTexture.getTexture()); // tell the torchSprite to use the texture setup ion the renderTexture torchTexture
}



//will be used to dim the light be provided by the torch as the player's fuel decreases
void TorchLight::torchUpdate(float maxFuel, float currentFuel)
{
	//this method works out the percentage of remianing fuel and makes it dark when keypoints in the percentage are hit
	//if the percentage of fuel left is over 90
	/*if ((currentFuel/ maxFuel) * 100 >= 90) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 120));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 150));
	}
	else if ((currentFuel / maxFuel) * 100 >= 80) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 135));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 150));
	}
	else if ((currentFuel / maxFuel) * 100 >= 70) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 150));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 175));
	}
	else if ((currentFuel / maxFuel) * 100 >= 60) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 175));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 190));
	}
	else if ((currentFuel / maxFuel) * 100 >= 50) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 190));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 205));
	}
	else if ((currentFuel / maxFuel) * 100 >= 40) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 205));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 220));
	}
	else if ((currentFuel / maxFuel) * 100 >= 30) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 220));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 235));
	}
	else if ((currentFuel / maxFuel) * 100 >= 20) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 235));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 250));
	}
	else if ((currentFuel / maxFuel) * 100 >= 10) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 245));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 255));
	}
	else if ((currentFuel / maxFuel) * 100 >= 1) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 250));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 255));
	}
	else if ((currentFuel / maxFuel) * 100 == 0) {
		//max brightness
		tempLightCircle.setFillColor(sf::Color(0, 0, 0, 255));
		tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 255));
	}*/

	//2nd method - work out the percentage and minus that from 255 and that is the light value. more gradual tick down, less bright
	float percentageFuelRemaining = (currentFuel / maxFuel) * 100;
	tempLightCircle.setFillColor(sf::Color(0, 0, 0, 255 - percentageFuelRemaining));
	tempLightCircle.setOutlineColor(sf::Color(0, 0, 0, 255 - percentageFuelRemaining + 10)); 
	//if the fuel is below 50% and the radius hasn't been set to be smaller, set a smaller radius
	if (percentageFuelRemaining < 50 && tempLightCircle.getRadius() > 30) {
		tempLightCircle.setRadius(30);
		tempLightCircle.setOrigin(tempLightCircle.getRadius(), tempLightCircle.getRadius());
	}
	else if (percentageFuelRemaining < 25 && tempLightCircle.getRadius() > 20) {
		tempLightCircle.setRadius(20);
		tempLightCircle.setOrigin(tempLightCircle.getRadius(), tempLightCircle.getRadius());
	}
}

void TorchLight::update(int playerX, int playerY, float maxFuel, float currentFuel)
{
	torchTexture.clear(sf::Color(0, 0, 0, 250));// reclear the renderTexture so that we don't get bright spots left behind
	tempLightCircle.setPosition(playerX +5  , playerY+5 ); //update the torch light position
	//redraw the texture  and set the updated position to the sprite

	//call this in the middle so we get the light value to draw
	torchUpdate(maxFuel, currentFuel); // call the torchUpdate so that gets done in the update as well, and pass the fuel so it can do it's calculation

	torchTexture.draw(tempLightCircle, sf::BlendMultiply); 
	torchTexture.display();
	torchSprite.setTexture(torchTexture.getTexture()); 

	
}
