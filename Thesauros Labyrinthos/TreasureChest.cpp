#include "TreasureChest.h"
#include "TextureLoader.h"
TreasureChest::TreasureChest(int x, int y)
{
	xPosition = x;
	yPosition = y;
	createSFML();
}

void TreasureChest::createSFML()
{
	rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	rectangle.setSize(sf::Vector2f(3.f, 3.f));
	rectangle.setOrigin(sf::Vector2f(1.5f, 1.5f));
	//rectangle.setFillColor(sf::Color::Yellow);
	rectangle.setTexture(&textureLoader.treaureChestTexture);
	textureSubrect.left = 32;
	textureSubrect.top = 31;
	textureSubrect.height = 32;
	textureSubrect.width = 31;
	rectangle.setTextureRect(textureSubrect);

	itemSprite.setPosition(sf::Vector2f(xPosition, yPosition - 1));
	itemSprite.setSize(sf::Vector2f(1.5f, 1.5f));
	itemSprite.setOrigin(sf::Vector2f(0.75f, 0.75f));
	//itemSprite.setFillColor(sf::Color(0, 0, 0, 0)); //set transparent until item is decided
	//itemSprite.setFillColor(sf::Color::Transparent);
}

void TreasureChest::createBody(b2World & world)
{

	//bodyDef
	chestBodyDef.type = b2_dynamicBody; 
	chestBodyDef.position.Set(xPosition / scale , yPosition / scale ); // set the position of the box2d body using the position of the object. divide by scale to convert from real measurements to pixel measurements
	chestBodyDef.angle = 0;
	chestBodyDef.fixedRotation = true; // prevent rotation
	chestBodyDef.userData = this;
	chestBody = world.CreateBody(&chestBodyDef); //create the body in the box2dworld and set it's def to be the one above

												//box2dShape
	chestShape.SetAsBox(1.5f / scale, 1.5f / scale);// create the box2d shape - the box- and set it's size. size is half of the sfml size becasue it uses half extents, and have to divide by scale to go from box2d's real world measurements to pixels

	//create the fixture
	chestFixture.shape = &chestShape;
	chestFixture.density = 1.f;
	chestFixture.friction = 0.0f;
	chestFixture.isSensor = true; //set to be a sensor
	chestFixture.filter.categoryBits = TREASURECHEST; // set the category to be TREASURECHEST
	chestFixture.filter.maskBits = PLAYER; //set to collide with player 
	chestBody->CreateFixture(&chestFixture);

	//create a second fixture to collide with walls (with just sensor it'll fall through the walls)
	chestFixture2.shape = &chestShape;
	chestFixture2.density = 1.f;
	chestFixture2.friction = 0.0f;
	chestFixture2.filter.categoryBits = TREASURECHEST; // set the category to be TREASURECHEST
	chestFixture2.filter.maskBits = WALL; //set to collide with player 
	chestBody->CreateFixture(&chestFixture2);
}

void TreasureChest::openChest(PlayerCharacter *playerCharacter)
{
	if (alreadyOpened == false) { // if chest hasn't been opened
		//select what item the player is going to get upon opening
 		selectedItem = rand() % 1000 + 1; // slecct a random number
		bool ItemAlreadyAquired = false;
		//base item on generated number
		if (selectedItem <= 100) { // if one to 10
			//health item
			printf("healthItem \n");
			if (playerCharacter->playerHealth < playerCharacter->playerMaxHealth) { // so long as not already full health
				playerCharacter->playerHealth++; //recover some player health
				alreadyOpened = true;
				itemSprite.setTexture(&textureLoader.healthBarTexture1); //display a heart texture
			}
		}
		else if (selectedItem > 100 && selectedItem <= 200) {
			//torch Fuel
			printf("torchFuel \n");
			playerCharacter->currentTorchFuel = playerCharacter->maxTorchFuel; //max out torch fuel

			alreadyOpened = true;
			itemSprite.setTexture(&textureLoader.torchTexture);
			//itemSprite.setFillColor(sf::Color(0, 0, 0, 255));
			
			
		}
		else if (selectedItem > 200 && selectedItem <= 300) {
			//shield fuel
			printf("shieldFuel \n");
			playerCharacter->shieldEnergy = playerCharacter->shieldEnergyMax; //max out shield energy
			alreadyOpened = true;
		}
		else if (selectedItem > 300 && selectedItem <= 400) {
			//Flamecloak
			printf("flameCloak \n");
			for (int i = 0; i < playerCharacter->AquiredItems.size(); i++) {
				if (playerCharacter->AquiredItems[i]->itemName == "FlameCloak") {
					ItemAlreadyAquired = true; //item al;ready aquired 
				}
			}
			if (ItemAlreadyAquired == false) { // if item isn't aquired yet
				playerCharacter->AquiredItems.push_back(std::shared_ptr<FlameCloakItem>(new FlameCloakItem())); //add the flame cloak item
				alreadyOpened = true;
				itemSprite.setTexture(&textureLoader.flameCloakTexture);
			}
		}
		else {
			printf("item = %i \n", selectedItem);
		}

		if (alreadyOpened == true) { //if chest is opened swap sprite
			textureSubrect.left = 0;
			textureSubrect.top = 34;
			textureSubrect.height = 29;
			textureSubrect.width = 32;
			rectangle.setTextureRect(textureSubrect);
		}
	}
}

void TreasureChest::update()
{
	//update sfml positions
	xPosition = chestBody->GetPosition().x * scale;
	yPosition = chestBody->GetPosition().y * scale;
	rectangle.setPosition(sf::Vector2f(xPosition, yPosition));
	itemSprite.setPosition(sf::Vector2f(xPosition + 0.75, yPosition - 2));
}
