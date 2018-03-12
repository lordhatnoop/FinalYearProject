#pragma once
#include "ItemHolder.h" //inlcude all of the items
#include "PlayerCharacter.h"
class TreasureChest {
public:
	TreasureChest(int x, int y);
	float xPosition;
	float yPosition;

	void createSFML();
	void createBody(b2World &world);
	void openChest(PlayerCharacter *playerCharacter);
	void update();
	sf::RectangleShape rectangle;
	sf::RectangleShape itemSprite;
	sf::IntRect textureSubrect;

	b2FixtureDef chestFixture;
	b2FixtureDef chestFixture2;
	b2BodyDef chestBodyDef;
	b2Body* chestBody;
	b2PolygonShape chestShape;

	int scale = 30;
	int selectedItem;


	bool alreadyOpened = false;
	//enum that holds the diffrent type of entities for collision filtering
	enum entityCategory {
		ENEMY = 0x0001,
		PLAYER = 0x0002,
		WALL = 0x0004,
		PLAYERPROJECTILE = 0x0005,
		ITEM = 0x0006,
		TREASURECHEST = 0x0007
	};
};