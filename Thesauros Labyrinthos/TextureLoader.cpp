#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
	//load the textures and store them
	//do this on creation so that the extern textureloader has all the textures loaded
	loadTextures();
}

void TextureLoader::loadTextures()
{
	//load in all of the Textures here so that we can use them everywhere else and don't have to keep reloading them
	//MazeTextures
	wallTexture.loadFromFile("Assets/Sprites/mazeWalls.png");
	fakeWallTexture.loadFromFile("Assets/Sprites/DarkerWall.png");
	floorTexture.loadFromFile("Assets/Sprites/mazeFloors.png");
	//menuBackgroundTexture.loadFromFile("Assets/Sprites/")

	//Title For Main Menu
	titleTexture.loadFromFile("Assets/Sprites/Menu/title.png");

	//healthBar Textures
	healthBarTexture9.loadFromFile("Assets/Sprites/UI/heart9.png");
	healthBarTexture8.loadFromFile("Assets/Sprites/UI/heart8.png");
	healthBarTexture7.loadFromFile("Assets/Sprites/UI/heart7.png");
	healthBarTexture6.loadFromFile("Assets/Sprites/UI/heart6.png");
	healthBarTexture5.loadFromFile("Assets/Sprites/UI/heart5.png");
	healthBarTexture4.loadFromFile("Assets/Sprites/UI/heart4.png");
	healthBarTexture3.loadFromFile("Assets/Sprites/UI/heart3.png");
	healthBarTexture2.loadFromFile("Assets/Sprites/UI/heart2.png");
	healthBarTexture1.loadFromFile("Assets/Sprites/UI/heart.png");
	
	shieldIcon.loadFromFile("Assets/Sprites/Shield.png");

	//PlayerTexture
	playerTexture.loadFromFile("Assets/Sprites/Player/Archer.png");
	
	//Item Related Textures
	flameCloakTexture.loadFromFile("Assets/Sprites/FireSprite.png");
	flameCloakGUITexture.loadFromFile("Assets/Sprites/UI/FlameCloakUI.png");

	torchTexture.loadFromFile("Assets/Sprites/torchSprite.png");

	treaureChestTexture.loadFromFile("Assets/Sprites/treasureChest.png");

	bombTexture.loadFromFile("Assets/Sprites/bomb_party_v4.png");
	bombGUITexture.loadFromFile("Assets/Sprites/bombIcon.png");

	hermesBootGUITexture.loadFromFile("Assets/Sprites/HermesBootsIcon.png");
	hermesBootTexture.loadFromFile("Assets/Sprites/HermesBoots.png");

	hermesHelmTexture.loadFromFile("Assets/Sprites/HermesHelmet.png");
	hermesHelmIcon.loadFromFile("Assets/Sprites/HermesHelmetIcon.png");

	goldenFleece.loadFromFile("Assets/Sprites/GoldenFleece.png");

	medusaHead.loadFromFile("Assets/Sprites/MedusaHead.png");
	medusaHeadIcon.loadFromFile("Assets/Sprites/MedusaHeadIcon.png");

	tidalWave.loadFromFile("Assets/Sprites/TidalWave.png");
	poseidonsTrident.loadFromFile("Assets/Sprites/Poseidons_trident");
	//traps
	spikeTexture.loadFromFile("Assets/Sprites/Traps/Spikes.png");
	Boulder.loadFromFile("Assets/Sprites/Traps/Boulder.png");
	GoldenIdol.loadFromFile("Assets/Sprites/Traps/Idol.png");
	boulder2.loadFromFile("Assets/Sprites/Traps/Boulder2.png");

		//background
	backgroundTexture.loadFromFile("Assets/Sprites/background.png");
	
	//enemy Textures
	griffinTexture.loadFromFile("Assets/Sprites/Enemies/Griffin.png");
	skeletonTexture.loadFromFile("Assets/Sprites/Enemies/Skeleton/Skeleton.png");
	medusaTexture.loadFromFile("Assets/Sprites/Enemies/Medusa/Medusa.png");
	ghostTexture.loadFromFile("Assets/Sprites/Enemies/Ghost.png");
	minotaurTexture.loadFromFile("Assets/Sprites/Enemies/MinotaurKing.png");
	//WIP - works, needs tweaking
	/*testRenderTExture.create(800, 450);
	testRenderTExture.clear(sf::Color(0, 0, 0, 200));
	
	sf::Texture texture;
	texture.loadFromFile("Assets/Sprites/light.png");
	sf::Sprite tempSprite(texture);
	testRenderTExture.draw(tempSprite, sf::BlendMultiply);
	sf::CircleShape lightCircle;
	lightCircle.setRadius(40);
	lightCircle.setPosition(400, 400);
	lightCircle.setFillColor(sf::Color(0, 0, 0, 100)); // black, beacuse of how sf::blendMultiply works, black cancles out black. the lower the alpha, the more light
	testRenderTExture.draw(lightCircle, sf::BlendMultiply);
	testRenderTExture.display();
	


	/////

	sf::Sprite sprite(testRenderTExture.getTexture());
	//darknessSprite = sprite;
	darknessSprite.setTexture(testRenderTExture.getTexture());
	sf::RenderStates renderState;
	renderState.blendMode = sf::BlendMultiply;
	*/
}
