#include "playerArrow.h"
#include "TextureLoader.h"
playerArrow::playerArrow(int x, int y, bool playerDirection)
{
	xPosition = x;
	yPosition = y;
	direction = playerDirection;
	createSFML(); // create the sfml on arrow creation

}

void playerArrow::createSFML()
{
	if (direction == false) {// if the direction is left
		xPosition = xPosition - 2; // update the xposition so it can be used for the body too
		arrowRect.setPosition(sf::Vector2f(xPosition, yPosition)); // set the arrow position to be slightly to the left of the player when created

		//set the texture to start from the right and use negative width and hie=ght to get texture flipped to face left
		arrowTextureRect.left = 1030; //998
		arrowTextureRect.width = -32; 
		arrowTextureRect.top = 347;
		arrowTextureRect.height = -5;
	}
	else if (direction == true) {//else if directiopn is right
		xPosition = xPosition + 2;
		arrowRect.setPosition(sf::Vector2f(xPosition, yPosition)); // set the arrow position to be slightly to the Right of the player when created

		//set texture sheet position
		arrowTextureRect.left = 998; 
		arrowTextureRect.width = 32;
		arrowTextureRect.top = 342;
		arrowTextureRect.height = 5;
	}

	arrowRect.setSize(sf::Vector2f(2.f, 1.f)); //set size, small cause arrow
	arrowRect.setOrigin(sf::Vector2f(1.f, 0.5f));
	arrowRect.setTexture(&textureLoader.playerTexture); //set texture to be the player texture from the loader. has arrows on it
	arrowRect.setTextureRect(arrowTextureRect); //tell it to use the setup textureRect sprite sheet positions
}

void playerArrow::createHitBox(b2World &myWorld)
{
	
	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale, yPosition / scale); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;
	//	BodyDef.userData = "playerCharacter"; // set the Userdata so that we can check what is colliding
	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

												//box2dShape
	Shape.SetAsBox(1.f / scale, 0.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

											  //create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.filter.categoryBits = PLAYERPROJECTILE; // set the category to be player projectile
	FixtureDef.filter.maskBits = ENEMY | WALL; //set to collide with walls and enemies - don't want it to collide with player or other arrows
	dynamicBody->CreateFixture(&FixtureDef);
	dynamicBody->SetGravityScale(0); //set the arrow to ignore gravity, because I just want them to fly in a straight line and not drop ( drop made it so that the speed had to be really high to get any distance before hitting the ground and made aiming awkward)
	if (direction == false) {//left
		dynamicBody->ApplyLinearImpulse(b2Vec2(-0.02f, 0.f), b2Vec2(2, 0.5), true); //apply negative force to the right side of the arrow to push arrow left // originally when we had drop on the arrows, applied some updwards force to it as well, to try and give arrows more distance, but the zero gravity worked better, making it easier to aim, letting me set a reasonable speed to the arrows, and just being more enjoyable to use
	}
	else if (direction == true) {
		dynamicBody->ApplyLinearImpulse(b2Vec2(0.02f, 0.f), b2Vec2(0, 0.5), true); //apply  force to the left side of the arrow to push arrow right
	}
}

void playerArrow::update()
{
	arrowRect.setPosition(dynamicBody->GetPosition().x * scale, dynamicBody->GetPosition().y * scale);
}
