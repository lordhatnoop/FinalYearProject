#include "Ghost.h"


Ghost::Ghost(int x, int y)
{//set the positions toi be those passed
	xPosition = x;
	yPosition = y;

	//setup the Ghosts Stats
	visionRange = 20.f;
	health = 2;

	animationTimer.restart(); //start the animation timer;

	//call createSfml to create the sprite
	createSFML();
	//decide the first goal point
	decidePath();
}

void Ghost::createSFML()
{
	rectangle.setSize(sf::Vector2f(5.f, 5.f));
	rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	rectangle.setOrigin(sf::Vector2f(2.5f, 2.5f));

	//setup the start position of the texture Subrect (rectangle that is the part of the sprite sheet we are using
	textureSubRect.left = 7.5; //left side of the rectangle
	textureSubRect.top = 8.5; // top of the rectangle for the sprite
	textureSubRect.height = 65.5; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
	textureSubRect.width = 39; // width

	rectangle.setTextureRect(textureSubRect); // set to use the texture SubRectangle
	rectangle.setTexture(&textureLoader.ghostTexture); //set texture
}

void Ghost::createCollisionBox(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale , yPosition / scale ); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.gravityScale = 0; //set gravity scale to 0 so that the ghost won't fall downwards
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;
	//BodyDef.userData = "Enemy"; // set the userdata for the bodydef so that we can check what is colliding
	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	

	Shape.SetAsBox(2.5f / scale, 2.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	

	 //create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.isSensor = true; //set sesnor so it can go through stuff but still detect collision
	FixtureDef.filter.categoryBits = ENEMY; //categoiry
	FixtureDef.filter.maskBits = PLAYER | PLAYERPROJECTILE | ITEM; //won't collide with other enemies, but will with player's, projectiles .
	dynamicBody->CreateFixture(&FixtureDef);
}

void Ghost::Animate()
{
	if (animationTimer.getElapsedTime().asSeconds() > 0.3) { // every third of a second 
		//if else if for left and right movement so we know whether to invert sprite or not.
		if (xPosition < pathGoal.x) { // moving right
			textureSubRect.width = 39; // width

			if (animationCounter == 0) {
				textureSubRect.left = 7.5; //left side of the rectangle
				textureSubRect.top = 8.5; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
			else if (animationCounter == 1) {
				textureSubRect.left = 109; //left side of the rectangle
				textureSubRect.top = 7; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
			else if (animationCounter == 2) {
				textureSubRect.left = 209.5; //left side of the rectangle
				textureSubRect.top = 5; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
		}
		else if (xPosition > pathGoal.x) { //moving left
			textureSubRect.width = -39; // width - need to invert to get left facing sprite
			//add width to all of the right facing left sides becasue of the invert

			if (animationCounter == 0) {
				textureSubRect.left = 46.5; //left side of the rectangle
				textureSubRect.top = 8.5; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
			else if (animationCounter == 1) {
				textureSubRect.left = 148; //left side of the rectangle
				textureSubRect.top = 7; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
			else if (animationCounter == 2) {
				textureSubRect.left = 248.5; //left side of the rectangle
				textureSubRect.top = 5; // top of the rectangle for the sprite
				textureSubRect.height = 68; // height of the rectangle, will be the bottom of the point on the sprite point minus the top
			}
		}

		animationCounter++; // update frame count after setting position in sprite s heet
		rectangle.setTextureRect(textureSubRect); // set to use the texture SubRectangle

		if (animationCounter > 3) {
			animationCounter = 0; //reset to 0 if gone past 3
		}
	}
}

void Ghost::decidePath()
{
	int x = rand() % 1000 + 1; //random point between 1 and 1000
	int y = rand() % 600 + 1; //random point between 1 and 600

	pathGoal = (sf::Vector2f(x, y)); //set goal to be the new points
}

void Ghost::update()
{
	//move
	b2Vec2 temp; //b2vec 2 which we will set the values for below
	if (floor(xPosition) < pathGoal.x) {
		temp.x = 3; // if need to go right, positive 3 to b2vec2 x component
	}
	else if (floor(xPosition) > pathGoal.x) {
		temp.x = -3; //opposite
	}
	//y check
	if (floor(yPosition) < pathGoal.y) {
		temp.y = 3; // if need to go down, positive 3 to b2vec2 x component
	}
	else if (floor(yPosition) > pathGoal.y) {
		temp.y = -3; //opposite
	}

	//now that we have the value, do the actual movement
	dynamicBody->SetLinearVelocity(temp);//set velkocity to be above decided values

	//update the sfml
	xPosition = dynamicBody->GetPosition().x * scale;
	yPosition = dynamicBody->GetPosition().y * scale;

	rectangle.setPosition(xPosition, yPosition);

	Animate();// ghost animate function - do after position update so that we have up to date readings

	if (sf::Vector2f(xPosition, yPosition) == pathGoal) { // if reached goal point
		decidePath();
	}

	if (health <= 0) {
		dead = true;
	}
}
