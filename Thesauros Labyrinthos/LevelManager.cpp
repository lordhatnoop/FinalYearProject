#pragma once
#include "LevelManager.h"
#include "TextureLoader.h"
#include <iostream>
#include "ExitCell.h"
#include "Skeleton.h"
#include "RopeItem.h"
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
		update(world); // for ingame state we just want to call the update function to keep updating the game
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
		//Enemy spawn test
		for (int i = 0; i < 10;) { //10 loops
			int EnemyX = rand() % 1599 + 1;
			int EnemyY = rand() % 899 + 1; //create X and Y postitons in the range of positions they can be 

			if (mazeGenerator.maze[EnemyX / 10][EnemyY / 10] == 0) {// check the randomised position against the maze to see if it's a floor (don't want  them to spawn in walls) (divide by 10 to change from screenposition to array position)
				skeletonsVector.push_back(std::shared_ptr<Skeleton>(new Skeleton(EnemyX, EnemyY))); //create a new enemy at that point if it's floor
				skeletonsVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. progress i here so that we're guarenteed 10 enemies
			}
		}
		/*
		std::shared_ptr<Skeleton> testEnemy = std::shared_ptr<Skeleton>(new Skeleton(mazeGenerator.startX - 10, mazeGenerator.startY ));
		testEnemy->createCollisionBox(world);
		skeletonsVector.push_back(testEnemy);
		*/
		
		std::shared_ptr<Medusa> testEnemy = std::shared_ptr<Medusa>(new Medusa(mazeGenerator.startX - 10, mazeGenerator.startY));
		testEnemy->createCollisionBox(world);
		medusaVector.push_back(testEnemy);
		
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

		std::shared_ptr<ExitCell> exit;
		exit = std::shared_ptr<ExitCell>(new ExitCell(mazeGenerator.endX, mazeGenerator.endY));
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

void LevelManager::update(b2World &World)
{
	//////////////////////////////////UPDATE//////////////////////////////////////////////////
	// call the player update function and pass delta time
	
	playerCharacter->update(dt, World);
	//update the arrows
	for (int i = 0; i < playerCharacter->arrowVector.size(); i++) {
		playerCharacter->arrowVector[i]->update();
		
	}
	//iterate through arrow vector using the iterator and remove them if destoryed, else carry on iterating. do this using iterators and seperate from thje other arrow update to avoid errors
	for (arrowIterator = playerCharacter->arrowVector.begin(); arrowIterator != playerCharacter->arrowVector.end();) {
		if ((*arrowIterator)->destroyed == true) { //if arrow destroyed
			World.DestroyBody((*arrowIterator)->dynamicBody); //destroy the body
			arrowIterator = playerCharacter->arrowVector.erase(arrowIterator); // remove from arrow vector
		}
		else {
			arrowIterator++;
		}
	}
	//update the camera and pass the playerposition so camera can follow.
	playerView->update(playerCharacter->dynamicBody->GetPosition().x * 30.f, playerCharacter->dynamicBody->GetPosition().y * 30.f);

	//update any skeletons
	for (int i = 0; i < skeletonsVector.size(); i++) {
		skeletonsVector[i]->update(playerCharacter);
	}
	//update any medusa
	for (int i = 0; i < medusaVector.size(); i++) {
		medusaVector[i]->update(playerCharacter);
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

	//PLayerHealth UI
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
	//draw medusas
	for (int i = 0; i < medusaVector.size(); i++) {
		window->draw(medusaVector[i]->rectangle);
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
	//playerDraw stuff - done here so that we don't have to pass player the window
	if (playerCharacter->activeItem != nullptr) { // so loong as active item isn't null
		window->draw(playerCharacter->activeItem->rectangle);//draw it
	}
	window->draw(playerCharacter->rectangle);
	window->draw(playerCharacter->shieldCircle);
	window->draw(playerCharacter->torch->torchSprite);
	
	
}


void LevelManager::upgradesMenu()
{
	playerCharacter->xPosition = 20000; //move the player off screen. don';t destory so we still have access to treasure, upgrades ,etc
	
	//create the upgrade menu buttons
	tgui::Button::Ptr UpgradeItemDamage = tgui::Button::create(); // create a button on the menu
	UpgradeItemDamage->setPosition(400, 300); // set the position of the button
	UpgradeItemDamage->setSize(800, 100); // set the start button size
	UpgradeItemDamage->setText("Upgrade Item Damage"); // set text
	UpgradeItemDamage->setTextSize(40); // set size of the text

	gui->add(UpgradeItemDamage); //add the start button to the gui so that it can be drawn and managed.

	UpgradeItemDamage->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

	//next button
	tgui::Button::Ptr UpgradeMaxHealth = tgui::Button::create(); // create a button on the menu
	UpgradeMaxHealth->setPosition(400, 450); // set the position of the button
	UpgradeMaxHealth->setSize(800, 100); // set the start button size
	UpgradeMaxHealth->setText("Upgrade Max Health"); // set text
	UpgradeMaxHealth->setTextSize(40); // set size of the text

	gui->add(UpgradeMaxHealth); //add the start button to the gui so that it can be drawn and managed.

	UpgradeMaxHealth->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button


	//next Button 
	tgui::Button::Ptr PlayerAddHealth = tgui::Button::create(); // create a button on the menu
	PlayerAddHealth->setPosition(400, 600); // set the position of the button
	PlayerAddHealth->setSize(800, 100); // set the start button size
	PlayerAddHealth->setText("Recover Health"); // set text
	PlayerAddHealth->setTextSize(40); // set size of the text

	gui->add(PlayerAddHealth); //add the start button to the gui so that it can be drawn and managed.

	PlayerAddHealth->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button
	
	//next button
	tgui::Button::Ptr ShieldEnergyUpgrade = tgui::Button::create(); // create a button on the menu
	ShieldEnergyUpgrade->setPosition(400, 600); // set the position of the button
	ShieldEnergyUpgrade->setSize(800, 100); // set the start button size
	ShieldEnergyUpgrade->setText("Shield Energy Upgrade"); // set text
	ShieldEnergyUpgrade->setTextSize(40); // set size of the text

	gui->add(ShieldEnergyUpgrade); //add the start button to the gui so that it can be drawn and managed.

	ShieldEnergyUpgrade->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

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
	else if (msg == "Exit Game") {
		window->close();; // tell the window to close becasue we want to exit
	}
	else if (msg == "Options") {
		std::cout << "Options Button pressed" << std::endl; // test it recognises the button
	}

	else if (msg == "Upgrade Item Damage") {
		if (playerCharacter->treasure >= itemDamageUpgradeCost * itemDamageUpgradeMultiplier) { // if the player has more treasure than the cost
			playerCharacter->ItemDamageMultiplier = playerCharacter->ItemDamageMultiplier + 0.25f;//upgrade the item damage multiplier
			playerCharacter->treasure = playerCharacter->treasure - itemDamageUpgradeCost * itemDamageUpgradeMultiplier; // remove the treasure
			itemDamageUpgradeMultiplier = itemDamageUpgradeMultiplier + 0.25f; // increase the cost multiplier so it'll cost more next time
		}
	}

	else if (msg == "Upgrade Max Health") {
		if (playerCharacter->treasure >= maxHealthUpgradeCost * maxHealthUpgradeMultiplier) { // if the player has more treasure than the cost
			playerCharacter->playerMaxHealth = playerCharacter->playerMaxHealth + 1;//upgrade the player's max health
			playerCharacter->playerHealth = playerCharacter->playerHealth + 1; //fill the extra health we just gained
			playerCharacter->treasure = playerCharacter->treasure - maxHealthUpgradeCost * maxHealthUpgradeMultiplier; // remove the gold
			itemDamageUpgradeMultiplier = itemDamageUpgradeMultiplier + 1.f; // increase the cost multiplier so it'll cost more next time
			
		}
	}

	else if (msg == "Recover Health") {
		if (playerCharacter->treasure >= 2000) { // if the player has more treasure than the cost
			if (playerCharacter->playerHealth < playerCharacter->playerMaxHealth) { // if the current health is less than max health we can add an extra health point
				playerCharacter->playerHealth = playerCharacter->playerHealth + 1; // add one health back to the player
				playerCharacter->treasure = playerCharacter->treasure - 2000; //remove the treasure
				 //no multiplier for this one. just high initial cost
			}
		}
	}

}
