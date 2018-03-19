#include "Treasure.h"

Treasure::Treasure(int x, int y)
{
	position = sf::Vector2f(x, y);
	decideValue();
	CreateSFML();
}

void Treasure::CreateSFML()
{
	rectangle.setSize(sf::Vector2f(1.f, 1.f));
	rectangle.setOrigin(sf::Vector2f(0.5f, 0.5f));
	rectangle.setPosition(position);
	
	//decide the colour based on value
	if (value < 10) {
		rectangle.setFillColor(sf::Color::Red);
	}
	else if (value > 10 && value < 20) {
		rectangle.setFillColor(sf::Color::Blue);
	}
	else if (value > 20 && value < 40) {
		rectangle.setFillColor(sf::Color::Green);
	}
	else if (value > 40 && value < 60) {
		rectangle.setFillColor(sf::Color::Cyan);
	}
	else if (value > 60) {
		rectangle.setFillColor(sf::Color::Yellow);
	}
}

void Treasure::CreateBody(b2World & myWorld)
{
	BodyDef.type = b2_dynamicBody; // set the playercharacter to ahve a dynamic body from box2d. will allow for movement and being effecetd by gravity and forces
	BodyDef.position.Set(position.x /30.f, position.y /30.f); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	BodyDef.angle = 0;
	BodyDef.fixedRotation = true; // prevent rotation
	BodyDef.userData = this;

	Body = myWorld.CreateBody(&BodyDef); //create the body in the box2dworld and set it's def to be the one above

	
	
	//shape
	Shape.SetAsBox(0.5f / 30.f, 0.5f / 30.f);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels


	 //create second sensor fixture for collision - want player to be able to walk thouhg it but don't want this to fall through walls so need the two fixtures
	sensorFixtureDef.shape = &Shape;
	sensorFixtureDef.density = 1.f;
	sensorFixtureDef.friction = 0.0f;
	sensorFixtureDef.isSensor = true; //sensor
	
	sensorFixtureDef.filter.categoryBits = TREASURE; //category
	sensorFixtureDef.filter.maskBits = PLAYER; //collide with  walls
	Body->CreateFixture(&sensorFixtureDef);

	//create the fixture for colliding with walls
	fixtureDef.shape = &Shape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.0f;
	fixtureDef.filter.categoryBits = TREASURE; //category
	fixtureDef.filter.maskBits = WALL; //collide with  walls
	Body->CreateFixture(&fixtureDef);

	

}

void Treasure::collect()
{
	collected = true;
}

void Treasure::decideValue()
{
	value = rand() % 100 + 1; //generate a random value between 1 and 100
}
