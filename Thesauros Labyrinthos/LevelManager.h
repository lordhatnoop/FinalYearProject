#pragma once
#include "stdafx.h"
#include "skeleton.h"
#include "Medusa.h"
#include "PlayerCharacter.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "mazeGeneration.h"
#include "MiniMap.h"

// enum for the states of the game
enum GameState {
	menuIdle,
	menuCreate,
	loadLevelState,
	inGameState,
	deleteLevelState,
	upgradeState,
	upgradeIdleState,
	deleteUpgradeMenuState
};

class LevelManager {

public:
	LevelManager(sf::RenderWindow *passedWindow, tgui::Gui *passedGUI);
	
	void LoadNextLevel(b2World &world);
	void DeleteCurrentLevel(b2World &world);
	void DeleteMainMenu();
	void DeleteUpgradeMenu();
	void update(b2World &myWorld); // will update all evrything in the currentlevel, will be called in main. cpp while loop

	GameState getCurrentState() { return currentState; }

	vector <std::shared_ptr<Skeleton>> skeletonsVector; //use sharedPtr because it handles deleting the pointers for me once they are removed from the vector
	vector <std::shared_ptr<Medusa>> medusaVector;
	//below creates a mazegenerator object and then talls it to call the generatemaze function
	MazeGeneration mazeGenerator;

	//create a playercharacter object and pass the start position of the maze generation since this will always be a floor tile (good place to start).
	PlayerCharacter* playerCharacter; //create a playercharacter here so that we can just move it off screen when we don't need it, because we want some of the information to stay constant between levels(treasure, upgrades,etc) and so can't destroy the player and create a new one eahc level

	//create a a camera object that will be used for the player's view. pass the player positions so that they can be used to set the center of the camera on the player
	Camera* playerView;

	//create a minimap object
	MiniMap* gameMiniMap;

	sf::Sprite title;

	void FSM(b2World &world); // Finite state machine


	 //TorchGUI//////////
	tgui::TextBox::Ptr torchFuelUI; // fuel UI for now
	sf::RectangleShape torchFuelUIBackgroundRed; // the background for the fuel ui
	sf::RectangleShape torchFuelUIBackgroundWhite; // the background for the fuel ui
	sf::RectangleShape torchFuelUIImage; // the image by the side of the torch fuel UI


	//background
	//create a background image
	sf::RectangleShape backgroundSprite;
	

private:
	
	void loadMenu();

	//upgrades menu stuff
	void upgradesMenu();
	void upgradesMenuIdle();
	//floats to hold the cost of the upgrades
	float itemDamageUpgradeCost = 800.f; //start cost
	float maxHealthUpgradeCost = 1000.f; //start cost
	float energyMaxUpgradeCost = 3000.f; //start cost
	//floats to hold the cost multiplier so that we can increase the cost for each upgrade
	float itemDamageUpgradeMultiplier = 1.f;
	float maxHealthUpgradeMultiplier = 1.f;
	float energyMaxUpgradeMultiplier = 1.f;

	void SignalManager(string msg);

	GameState currentState; // hold the current state we are in

	sf::RenderWindow* window; // pointer to the sf window we will pass
	
	//GUI//////
	tgui::Gui* gui; // pointer to the gui we will pass
	tgui::TextBox::Ptr treasureUI; // textbox UI
	tgui::TextBox::Ptr healthUI; // healthUI

	//upgrades menu GUI extras
	tgui::TextBox::Ptr ItemDamageCostUI;
	tgui::TextBox::Ptr MaxHealthCostUI;
	tgui::TextBox::Ptr AddHealthCostUI;
	tgui::TextBox::Ptr EnergyRefillCostUI;
	tgui::TextBox::Ptr EnergyMaxUpgradeCostUI;
	tgui::TextBox::Ptr TorchRefillCostUI;
	

	int levelCounter = 0; // used to track which level we are on 

	sf::Clock updateclock;
	float dt = 1 / 60.f; //delta time - frames above 60, so no need to do fancy calculation 
	vector<std::shared_ptr<playerArrow>>::iterator arrowIterator;
};