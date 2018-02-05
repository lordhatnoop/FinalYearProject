#pragma once
#include "stdafx.h"

class TextureLoader {
public:
	
	TextureLoader();

	//wall and floor texture
	sf::Texture wallTexture;
	sf::Texture wallTexture2;
	sf::Texture floorTexture;
	sf::Texture menuBackgroundTexture;
	sf::Texture titleTexture;
	sf::Texture healthBarTexture9;
	sf::Texture healthBarTexture8;
	sf::Texture healthBarTexture7;
	sf::Texture healthBarTexture6;
	sf::Texture healthBarTexture5;
	sf::Texture healthBarTexture4;
	sf::Texture healthBarTexture3;
	sf::Texture healthBarTexture2;
	sf::Texture healthBarTexture1;
	sf::Texture skeletonTexture;
	//test for torch / spotlight
	sf::RenderTexture testRenderTExture;
	sf::Sprite darknessSprite;
	//
	void loadTextures();

};
//declare as an extern so that we can use it in anyclass with textureLoader.h included, meaing we can load all the textures here
//and send them to any other object that needs them
extern TextureLoader textureLoader;
