#pragma once
#include "LevelManager.h"
#include "TextureLoader.h"
#include <iostream>
#include "ExitCell.h"
#include "Skeleton.h"
#include "Cell.h"
using namespace std;

LevelManager::LevelManager(sf::RenderWindow *passedWindow, tgui::Gui *passedGUI)
{
	window = passedWindow; // set the window to be the pointer to our gamewinodw in main.cpp
	gui = passedGUI; // set the gui in this class to be a pointer to the passed one from main


	//initilise the playerCharacter and the playerview and just set them off-screen for now, so that they're not visible but exist so we can use them.
	//we don't want these to be deconstructed until the game ends so we'll keep moving them off screen when changing levels, going to menus ,etc
	playerCharacter = new PlayerCharacter(2000,2000);
	playerView = new Camera(playerCharacter->xPosition, playerCharacter->yPosition);

	currentState = menuCreate; //set the state to be menustate so that we load the menu 
}

void LevelManager::FSM(b2World &world)
{
	//create a switch that uses the currentstate as it's parameter
	//Acts as the controller for the Finite State Machine
	switch (currentState) {
		//first state is the menu. call the load menu class
	case menuCreate:

		loadMenu();

		break;

	case loadLevelState:
		LoadNextLevel(world);
		break;
	case deleteLevelState:
		DeleteCurrentLevel(world);
		break;
	case inGameState:
		update(); // for ingame state we just want to call the update function to keep updating the game
		break;

	case upgradeState:

		break;
	}
}

void LevelManager::loadMenu()
{

	title.setTexture(textureLoader.titleTexture); // create a title sprite and get the texture for it from the textureLoader
	title.setPosition(100, 0);
	title.setScale(sf::Vector2f(1.5, 1.5)); // set the scale of the title image to be 1.5 x the image size

	tgui::Button::Ptr StartButton = tgui::Button::create(); // create a button on the menu
	StartButton->setPosition(400, 300); // set the position of the button
	StartButton->setSize(800, 100); // set the start button size
	StartButton->setText("Start Game"); // set text
	StartButton->setTextSize(40); // set size of the text

	gui->add(StartButton); //add the start button to the gui so that it can be drawn and managed.

	StartButton->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button



	tgui::Button::Ptr OptionsButton = tgui::Button::create(); // create a button on the menu
	OptionsButton->setPosition(400, 450); // set the position of the button
	OptionsButton->setSize(800, 100); // set the  button size
	OptionsButton->setText("Options");// set text to display on the button
	OptionsButton->setTextSize(40); // set size of the text

	gui->add(OptionsButton); //add the Options button to the gui so that it can be drawn and managed.
	OptionsButton->connect(std::string("pressed"), &LevelManager::SignalManager, this);


	tgui::Button::Ptr ExitButton = tgui::Button::create(); // create a button on the menu
	ExitButton->setPosition(400, 600); // set the position of the button
	ExitButton->setSize(800, 100); // set the  button size
	ExitButton->setText("Exit Game"); // set text
	ExitButton->setTextSize(40); // set size of the text


	gui->add(ExitButton); //add the Exit button to the gui so that it can be drawn and managed.
	ExitButton->connect(std::string("pressed"), &LevelManager::SignalManager, this);



	currentState = menuIdle; // set the state to be idle so that we don't keep creating the menu over and over again.
}

void LevelManager::DeleteMainMenu()
{
	gui->removeAllWidgets(); // remove all the widgets so that menu doesn't get drawn anymore
}

void LevelManager::LoadNextLevel(b2World &world)
{
	levelCounter++; // start by updating the level counter

	if (levelCounter % 3 == 0) { // divides level counter by 3 to see if it is a multiple of 3. will be 0 if it is. so if level is multiple of 3, do contained
		currentState = upgradeState; // set the current state to be the upgrades state every third level, to give the player a chance to spend their gold on upgrades;
	}
	else { // else do the normal load next level code
		mazeGenerator.generateMaze(world); // generate a maze

		//set the player's position on the screen
		playerCharacter->xPosition = mazeGenerator.startX;
		playerCharacter->yPosition = mazeGenerator.startY;
		//////////////////////////SKELETON TEST/////////////////////////////////////////
		Skeleton *testEnemy = new Skeleton(mazeGenerator.startX - 10, mazeGenerator.startY );
		testEnemy->createCollisionBox(world);
		skeletonsVector.push_back(testEnemy);
		
		//GUI CREATION//////////////////////////////////////////
		if (gui->getWidgets().size() <= 0) {//if the gui is empty 
		//create the gui for the level and add it to the GUI holder. do this here instead of update becasue we don't want to keep cvreating them over and over again
			treasureUI = tgui::TextBox::create();
			treasureUI->setPosition(0, 0); // set the textBox to be positioned at 0,0
			treasureUI->setSize(200, 25); // set the size
			treasureUI->setTextSize(16); // set the font size
			treasureUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
			treasureUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

			gui->add(treasureUI); // add the treasure UI to the gui
			
			///////HEALTH UI///////////////
			healthUI = tgui::TextBox::create();
			healthUI->setPosition(200, 0);
			healthUI->setSize(200, 25);
			healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture2);
			
			gui->add(healthUI);

			///////////////////////FUEL UI////////////////////////////////////
			//the textbox portion
			torchFuelUI = tgui::TextBox::create();
			torchFuelUI->setPosition(400, 0); // set the textBox to be positioned at 400,0
			torchFuelUI->setSize(600, 25); // set the size
			torchFuelUI->setTextSize(16); // set the font size
			torchFuelUI->getRenderer()->setBackgroundColor(sf::Color::Transparent); // set colour
			torchFuelUI->getRenderer()->setBorderColor(sf::Color::Green);//border colour

			
			//two below are awakward. normal rectangles, so get drawn normally to the window, rather than to the view like the gui stuff using TGUI. have to psoition them relative to the player, and make them very small to fit the zoomed in view
			//the red backgroundportion
			torchFuelUIBackgroundRed.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
			torchFuelUIBackgroundRed.setSize(sf::Vector2f(37.5f,2.8f));
			torchFuelUIBackgroundRed.setFillColor(sf::Color::Red);

			//the white backgroundportion - create two background portions so that this one is left behind as the red one updates and shrinks with the current fuel
			torchFuelUIBackgroundWhite.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
			torchFuelUIBackgroundWhite.setSize(sf::Vector2f(37.5f, 2.8f));
			torchFuelUIBackgroundWhite.setFillColor(sf::Color::White);

			gui->add(torchFuelUI);

		}

		ExitCell *exit;
		exit = new ExitCell(mazeGenerator.endX, mazeGenerator.endY);
		mazeGenerator.cellsVector.push_back(exit);

		
		

		playerCharacter->createCollisionBox(world);
		currentState = inGameState;// set the current state to be ingame state so that we can start updating the game
	}
}

void LevelManager::DeleteCurrentLevel(b2World &world)
{
	world.DestroyBody(playerCharacter->dynamicBody); // destroy the player character body
	//playerCharacter->dynamicBody = nullptr; // set body null
	playerCharacter->currentTorchFuel = playerCharacter->maxTorchFuel; //reset the amount of fuel the player has for the torch


	mazeGenerator.cellsVector.clear(); // clear the cells vector so that we can create the new maze
	skeletonsVector.clear(); //clear the skeletons vector
	gui->removeAllWidgets(); //remove all the gui

	currentState = loadLevelState; // transition to load level state to load the next level
}

void LevelManager::update()
{
	//////////////////////////////////UPDATE//////////////////////////////////////////////////
	// call the player update function and pass delta time
	
	playerCharacter->update(dt);
	
	//update the camera and pass the playerposition so camera can follow.
	playerView->update(playerCharacter->dynamicBody->GetPosition().x * 30.f, playerCharacter->dynamicBody->GetPosition().y * 30.f);


	for (int i = 0; i < skeletonsVector.size(); i++) {
		skeletonsVector[i]->update(playerCharacter);
	}

	//////////////////////GUI UPDATE//////////////////////////////////////////////////////////////////

	//treasure UI
	treasureUI->setText("Treasure: " + to_string(playerCharacter->treasure));//set the treasure ui string to be the amount of treasure the player has

	//TorchFuel UI
	torchFuelUI->setText("Fuel Remaining: " + to_string(playerCharacter->currentTorchFuel) + " / " + to_string(playerCharacter->maxTorchFuel)); //set the text for the fuel UI to display the current and max fuel. 
	float redBarSize = (playerCharacter->currentTorchFuel / playerCharacter->maxTorchFuel) * 37.5f; // work out the new size of the red portion of the UI background by getting the fraction and multiplying it by the max width
	
	torchFuelUIBackgroundRed.setSize(sf::Vector2f(redBarSize, 2.9f));
	//have to keep updating the position based on the player's position becasue opf how the view follows the player, and these are just normal rectnagles, so don't get drawn staright to the view
	torchFuelUIBackgroundRed.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
	torchFuelUIBackgroundWhite.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);

	//PLayerHEalth UI
	//check the player's health and update the UI based on that
	if (playerCharacter->playerHealth == 1) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture1); //change to the one heart texture
		healthUI->setSize(22.2f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 2) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture2);
		healthUI->setSize(44.4f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 3) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture3);
		healthUI->setSize(66.6f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 4) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture4);
		healthUI->setSize(88.8f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 5) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture5);
		healthUI->setSize(111.0f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 6) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture6);
		healthUI->setSize(133.2f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 7) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture7);
		healthUI->setSize(155.4f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 8) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture8);
		healthUI->setSize(177.6f, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	else if (playerCharacter->playerHealth == 9) {
		healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture9);
		healthUI->setSize(200, 25); // change the size of the bar so that the hearts don't get sacled up
	}
	
	/////////////////////////DRAW///////////////////////////////////////////////////
	//set the window to use the cameraview
	window->setView(playerView->cameraView);

	//draw all the maze cells
	for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
		window->draw(mazeGenerator.cellsVector[i]->rectangle);
	}

	//draw the skeletons
	for (int i = 0; i < skeletonsVector.size(); i++) {
		window->draw(skeletonsVector[i]->rectangle);
	}

	//for testing
	//if L is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) { //search vector
			if (mazeGenerator.cellsVector[i]->cellType == "Exit") { // find the exit cell
				mazeGenerator.cellsVector[i]->ExitReached = true;
			}
		}
	}
	
	for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) { //search vector
		if (mazeGenerator.cellsVector[i]->cellType == "Exit") { // find the exit cell
			if (mazeGenerator.cellsVector[i]->ExitReached == true) { // exit is reached
				currentState = deleteLevelState;
				//DeleteCurrentLevel(); //delete the current Level

			}
		}
	}

	for (int i = 0; i < playerCharacter->arrowVector.size(); i++) { //get the arrow vector size and iterate through
		window->draw(playerCharacter->arrowVector[i]->arrowRect);
	}
	//draw stuff
	
	window->draw(playerCharacter->rectangle);
	//window->draw(playerCharacter->torch->torchSprite);

	
}


void LevelManager::upgradesMenu()
{

}


void LevelManager::SignalManager(string msg)
{
	//check the msg passed and do action based on that.
	if (msg == "Start Game") {
		std::cout << " Start Button pressed" << std::endl; // test it recognises the button
		DeleteMainMenu(); // delete the main menu so that it doesn't keep displaying in the game
		//LoadNextLevel(); // load the level
		currentState = loadLevelState;
	}
	if (msg == "Exit Game") {
		window->close();; // tell the window to close becasue we want to exit
	}
	if (msg == "Options") {
		std::cout << "Options Button pressed" << std::endl; // test it recognises the button
	}
}
