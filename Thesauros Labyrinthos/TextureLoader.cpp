#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
	//load the textures and store them
	//do this on creation so that the extern textureloader has all the textures loaded
	loadTextures();
}

void TextureLoader::loadTextures()
{
	wallTexture.loadFromFile("Assets/Sprites/mazeWalls.png");
	wallTexture2.loadFromFile("Assets/Sprites/mazeWall2.png");
	floorTexture.loadFromFile("Assets/Sprites/mazeFloors.png");
	//menuBackgroundTexture.loadFromFile("Assets/Sprites/")
	titleTexture.loadFromFile("Assets/Sprites/Menu/title.png");
	healthBarTexture9.loadFromFile("Assets/Sprites/UI/heart9.png");
	healthBarTexture8.loadFromFile("Assets/Sprites/UI/heart8.png");
	healthBarTexture7.loadFromFile("Assets/Sprites/UI/heart7.png");
	healthBarTexture6.loadFromFile("Assets/Sprites/UI/heart6.png");
	healthBarTexture5.loadFromFile("Assets/Sprites/UI/heart5.png");
	healthBarTexture4.loadFromFile("Assets/Sprites/UI/heart4.png");
	healthBarTexture3.loadFromFile("Assets/Sprites/UI/heart3.png");
	healthBarTexture2.loadFromFile("Assets/Sprites/UI/heart2.png");
	healthBarTexture1.loadFromFile("Assets/Sprites/UI/heart.png");
	skeletonTexture.loadFromFile("Assets/Sprites/Enemies/Skeleton/Skeleton.png");
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
