#include "PlayerCharacter.h"
#include "textureLoader.h"
PlayerCharacter::PlayerCharacter(int x, int y)
{
	xPosition = x;
	yPosition = y;
	createSFML();
	//createCollisionBox(myWorld);
	torch = new TorchLight(xPosition,yPosition);
	
}

void PlayerCharacter::createSFML()
{
	rectangle.setSize(sf::Vector2f(6.f, 6.f));
	rectangle.setOrigin(sf::Vector2f(3.f, 3.f));
	rectangle.setFillColor(sf::Color::Green); // sets the colour of the rectangle to be green // placeholder until texture
	rectangle.setPosition(xPosition, yPosition); // set the postion of the rectangle to be the position passed
	//rectangle.setTextureRect(25,25)
	//rectangle.setTexture(&texture, false);
}

void PlayerCharacter::createCollisionBox(b2World &myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale + 0.169 , yPosition  / scale + 0.169);
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above
	
	//box2dShape
	objectShape.SetAsBox(3.f / scale, 3.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

	//create the fixture
	objectFixtureDef.shape = &objectShape;
	objectFixtureDef.density = 1.f;
	objectFixtureDef.friction = 0.5f;
	dynamicBody->CreateFixture(&objectFixtureDef);
	
}

void PlayerCharacter::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
		playerHealth--;//reduce playerHealth by 1
		
	}
	//move player left by reducing the xposition and setting the posiiton of the rectangle again.
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//xPosition -= 5;
		//dynamicBody->ApplyForce(b2Vec2(-5, 0), b2Vec2(0, 0), true);
		dynamicBody->SetLinearVelocity(b2Vec2(-5, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//xPosition += 5;
		//dynamicBody->ApplyForce(b2Vec2(5, 0), b2Vec2(0, 0), true);
		dynamicBody->SetLinearVelocity(b2Vec2(5, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//yPosition += 5;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//yPosition -= 5;
	}
	else {
		dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // if no buttons are being pressed, no velocity
	}
		
	
	
	//rectangle.setPosition(xPosition, yPosition);
	//update the xPOsition and Yposiition so that it can still be sued for the torch
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;
	//update the rectangle position to be the new position if there is one
	rectangle.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale);
	//rectangle.setRotation((180 / b2_pi) * dynamicBody->GetAngle()); // sets the rectangle's angle to follow the body so that the graphics and collision box sync up. have to times the angle by (180 / b2_pi) because box2d use radians and we have to change that to degrees for sfml.
	


	torchCountdown();
	//call the torch update so that it updates along with the player
	torch->update(xPosition,yPosition,maxTorchFuel,currentTorchFuel);



}

void PlayerCharacter::torchCountdown()
{
	if (currentTorchFuel > 0) {
		currentTorchFuel -= 0.05; // just minus 0.5 perupdate for a test
	}
}

