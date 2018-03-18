#pragma once
#include "stdafx.h"
 //class that loads in all of the textures and holds them
class TextureLoader {
public:
	
	TextureLoader();

	//The various textures in the game


	//maze cell textures
	sf::Texture wallTexture;
	sf::Texture fakeWallTexture;
	sf::Texture floorTexture;

	//title and background textures
	sf::Texture menuBackgroundTexture;
	sf::Texture titleTexture;
	sf::Texture backgroundTexture;

	//Health bar textures
	sf::Texture healthBarTexture9;
	sf::Texture healthBarTexture8;
	sf::Texture healthBarTexture7;
	sf::Texture healthBarTexture6;
	sf::Texture healthBarTexture5;
	sf::Texture healthBarTexture4;
	sf::Texture healthBarTexture3;
	sf::Texture healthBarTexture2;
	sf::Texture healthBarTexture1;

	//enemy textures
	sf::Texture medusaTexture;
	sf::Texture skeletonTexture;
	sf::Texture griffinTexture;

	//player textures
	sf::Texture playerTexture;
	sf::Texture torchTexture;
	
	//item related textures
	sf::Texture flameCloakTexture;
	sf::Texture flameCloakGUITexture;
	sf::Texture treaureChestTexture;

	//test for torch / spotlight
	sf::RenderTexture testRenderTExture;
	sf::Sprite darknessSprite;
	//
	void loadTextures();

};
//declare as an extern so that we can use it in anyclass with textureLoader.h included, meaing we can load all the textures here
//and send them to any other object that needs them
extern TextureLoader textureLoader;
