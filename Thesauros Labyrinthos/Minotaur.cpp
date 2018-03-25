#include "Minotaur.h"

Minotaur::Minotaur(int x, int y)
{
	xPosition = x;
	yPosition = y;
	pathCheckTimer.restart(); //start the timer
	health = 100; //huge amount of health - supposed to be sub boss 

	createSFML(); //create sprite
}

void Minotaur::createSFML()
{
	rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	rectangle.setSize(sf::Vector2f(4.f, 8.f));
	rectangle.setOrigin(sf::Vector2f(2.f, 4.f)); //half of size to sync with box2d
	rectangle.setTexture(&textureLoader.minotaurTexture); //set texture

	//setup part of sprite sheet to use
	textureSubRect.left = 6.5;
	textureSubRect.width = 38;
	textureSubRect.top = 7;
	textureSubRect.height = 80;

	rectangle.setTextureRect(textureSubRect);
}

void Minotaur::createCollisionBox(b2World & myWorld)
{
	//bodyDef
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(xPosition / scale , yPosition / scale ); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.gravityScale = 0; //no gravity
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;
	//BodyDef.userData = "Enemy"; // set the userdata for the bodydef so that we can check what is colliding
	dynamicBody = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	//box2dShape
	//need to setup the vertices for the body manually becasue we want to cut the edges of the square to prevent an error with box2d where moving across multiple flat bodies can cause you to become stuck
	b2Vec2 verticices[8];
	verticices[7].Set(-1.6f / scale, 4.f / scale);
	verticices[6].Set(-2.f / scale, 3.f / scale);
	verticices[5].Set(-2.f / scale, -3.f / scale);
	verticices[4].Set(-1.6f / scale, -4.f / scale);
	verticices[3].Set(1.6f / scale, -4.f / scale);
	verticices[2].Set(2.f / scale, -3.f / scale);
	verticices[1].Set(2.f / scale, 3.f / scale);
	verticices[0].Set(1.6f / scale, 4.f / scale);

	//Shape.SetAsBox(2.f / scale, 4.f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels
	Shape.Set(verticices, 8); //set shape to use our vertices

	//create the fixture
	FixtureDef.shape = &Shape;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.filter.categoryBits = ENEMY; //categoiry
	FixtureDef.filter.maskBits = PLAYER | WALL | PLAYERPROJECTILE | ITEM; //won't collide with other enemies, but will with player's, projectiles and walls.
	dynamicBody->CreateFixture(&FixtureDef);
}

string Minotaur::getName()
{
	return "Minotaur";
}

void Minotaur::update(PlayerCharacter * player,  MazeGeneration *maze)
{
	dynamicBody->SetLinearVelocity(b2Vec2(0, 0)); // set 0 velocity to get rid of previous velocities and stop any sliding
	if (!path.empty()) {
		short x;
		short y;
		maze->inverseIndex(path.front(), &x, &y); // use inverse index on  the front of the path to get the x and y positions from the Cell number of the next path point
		
		move(x,y); //move 
	
	}

	//keep updating the sfml rectangle to follow the body.
	xPosition = dynamicBody->GetPosition().x *scale + 2.f;
	yPosition = dynamicBody->GetPosition().y *scale;
	rectangle.setPosition(xPosition, yPosition);

	updatePath(player, maze); //call the path update
	checkDead(); //check if dead

	
}

void Minotaur::updatePath(PlayerCharacter * player, MazeGeneration *maze)
{
	if(pathCheckTimer.getElapsedTime().asSeconds() > 5){ // limit how frequently we check
   		if (player->xPosition != xPosition && player->yPosition != yPosition) { //don't check if already on top of the player to prevent errors
			//check which side player is on. if right go to cell 1 more than player to run through them, otherwise cell 1 less than them to run through them
			if (player->xPosition < xPosition) { //left
				path = maze->GeneratePath(xPosition / 10, yPosition / 10, (player->xPosition - 10) / 10, player->yPosition / 10); //keep updating the path to the player
				pathCheckTimer.restart();
			}
			else { //right 
				path = maze->GeneratePath(xPosition / 10, yPosition / 10, (player->xPosition + 10) / 10, player->yPosition / 10); //keep updating the path to the player
				pathCheckTimer.restart();
			}
		}
	}
}

void Minotaur::checkDead()
{
	if (health <= 0) {
		dead = true;
	}
}

void Minotaur::move(short x , short y)
{
	xPosition = dynamicBody->GetPosition().x *scale - 2.f;
	yPosition = dynamicBody->GetPosition().y * scale - 4.f;
	//prioritise vertical movement
	b2Vec2 velocity = b2Vec2(0, 0); //default veloctiy. only change if  one of the below is true
	int roundedY = floorY(yPosition);
	int roundedX = roundUp(xPosition, 10);
	
	if (floorY(xPosition) < x *10) { // if less than goal x move right
		velocity.x = 2;
	}
	else if (floorY(xPosition) > x* 10) { // else if more than 
		velocity.x = -2;
	}
	else if(floorY(yPosition) > y * 10) { //next priority is jumping
    	velocity.y = -20; //jump
	}
	else if (floorY(yPosition) < y * 10) { //next priority is jumping
		velocity.y = 20; //jump
	}
	//don't check for down y velocity because if the pathfining is working, when we need to go down, there will be holes, and gravity will handle it

	//apply the velocity
	dynamicBody->SetLinearVelocity(velocity);

	//check if path point reached and remove if it is
	if (floorY(xPosition) == x * 10) {
		if (floorY(yPosition) >= y * 10 && floorY(yPosition) < y * 10 + 1) {
			path.pop_front();
		}
	}
	
}
