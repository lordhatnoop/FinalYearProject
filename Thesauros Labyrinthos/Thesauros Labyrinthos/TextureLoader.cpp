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
