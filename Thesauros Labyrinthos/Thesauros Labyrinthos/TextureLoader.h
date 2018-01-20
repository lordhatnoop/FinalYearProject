#pragma once
#include "stdafx.h"

class TextureLoader {
public:
	
	TextureLoader();

	//wall and floor texture
	sf::Texture wallTexture;
	sf::Texture wallTexture2;
	sf::Texture floorTexture;


	//test for torch / spotlight
	sf::RenderTexture testRenderTExture;
	sf::Sprite darknessSprite;
	//
	void loadTextures();

};
//declare as an extern so that we can use it in anyclass with textureLoader.h included, meaing we can load all the textures here
//and send them to any other object that needs them
extern TextureLoader textureLoader;
