#pragma once
#include "stdafx.h"
#include "GameCharacters.h"
#include "PlayerCharacter.h"
#include "Cell.h"
#include "Skeleton.h"
#include "Medusa.h"
class MiniMap {
public:
	MiniMap(vector <std::shared_ptr<Cell>> cellsVector, vector <std::shared_ptr<Skeleton>> Skeletons, vector <std::shared_ptr<Medusa>> Medusa);
	void MiniMapUpdate(sf::RenderWindow *passedWindow, vector <std::shared_ptr<Cell>> cellsVector, vector <std::shared_ptr<Skeleton>> Skeletons, vector <std::shared_ptr<Medusa>> Medusa, PlayerCharacter* playerCharacter);

	//the view that will be used foir the minimap
	sf::View minimapView;

	vector<sf::RectangleShape> miniMapWallsVector; // vector to hold the simplified representation of the minimap objects
	vector<sf::RectangleShape> miniMapSkeletonvector;
	vector<sf::RectangleShape> miniMapMedusavector;
	sf::RectangleShape playerMiniMap;
	sf::RectangleShape miniMapBorder;
};