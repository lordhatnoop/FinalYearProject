#pragma once
#include "stdafx.h"
#include "skeleton.h"
#include "PlayerCharacter.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "mazeGeneration.h"


// enum for the states of the game
enum GameState {
	menuIdle,
	menuCreate,
	loadLevelState,
	inGameState,
	upgradeState
};

class LevelManager {

public:
	LevelManager(sf::RenderWindow *passedWindow, tgui::Gui *passedGUI);
	
	void LoadNextLevel(b2World &world);
	void DeleteCurrentLevel();
	void DeleteMainMenu();
	void DeleteUpgradeMenu();
	void update(); // will update all evrything in the currentlevel, will be called in main. cpp while loop

	GameState getCurrentState() { return currentState; }

	vector <Skeleton*> skeletonsVector;

	//below creates a mazegenerator object and then talls it to call the generatemaze function
	MazeGeneration mazeGenerator;

	//create a playercharacter object and pass the start position of the maze generation since this will always be a floor tile (good place to start).
	PlayerCharacter* playerCharacter;

	//create a a camera object that will be used for the player's view. pass the player positions so that they can be used to set the center of the camera on the player
	Camera* playerView;

	sf::Sprite title;

	void FSM(b2World &world); // Finite state machine

private:
	
	void loadMenu();
	void upgradesMenu();

	void SignalManager(string msg);

	GameState currentState; // hold the current state we are in

	sf::RenderWindow* window; // pointer to the sf window we will pass
	
	tgui::Gui* gui; // pointer to the gui we will pass
	tgui::TextBox::Ptr treasureUI; // textbox UI
	tgui::TextBox::Ptr healthUI; // healthUI


	int levelCounter = 0; // used to track which level we are on 



};