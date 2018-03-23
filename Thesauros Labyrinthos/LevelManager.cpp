#pragma once
#include "LevelManager.h"
#include "TextureLoader.h"
#include <iostream>
#include "ExitCell.h"
#include "Skeleton.h"
#include "RopeItem.h"
#include "Cell.h"
#include "soundManager.h"
#include <fstream>
#include <iostream>



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
		upgradesMenu();
		break;
	case upgradeIdleState:
		upgradesMenuIdle();
		break;
	case deleteUpgradeMenuState:
		DeleteUpgradeMenu();
		break;
	case loadEscapeRunLevelState:
		loadBackwardsLevel(world);
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

	//play the main menu theme music
	soundManager.mainMenuMusic.play();

	currentState = menuIdle; // set the state to be idle so that we don't keep creating the menu over and over again.
}

void LevelManager::DeleteMainMenu()
{
	soundManager.mainMenuMusic.stop();
	gui->removeAllWidgets(); // remove all the widgets so that menu doesn't get drawn anymore
}

void LevelManager::LoadNextLevel(b2World &world)
{
	levelCounter++; // start by updating the level counter
	mazeGenerator.levelCounter = levelCounter;
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
		/*
		for (int i = 0; i < 10;) { //10 loops
			int EnemyX = rand() % 1599 + 1;
			int EnemyY = rand() % 899 + 1; //create X and Y postitons in the range of positions they can be 

			if (mazeGenerator.maze[EnemyX / 10][EnemyY / 10] == 0) {// check the randomised position against the maze to see if it's a floor (don't want  them to spawn in walls) (divide by 10 to change from screenposition to array position)
				skeletonsVector.push_back(std::shared_ptr<Skeleton>(new Skeleton(EnemyX, EnemyY))); //create a new enemy at that point if it's floor
				skeletonsVector.back()->createCollisionBox(world); //create the skeletons collision box (box2d)
				i++; // progress i. progress i here so that we're guarenteed 10 enemies
			}
		}*/
		spawnManager.spawnEnemies(world, mazeGenerator, skeletonsVector, medusaVector, griffinVector, ghostVector); //use the spawn manager to create the enemies
		
		//test Skeleton
		/*
		std::shared_ptr<Skeleton> testEnemy = std::shared_ptr<Skeleton>(new Skeleton(mazeGenerator.startX - 10, mazeGenerator.startY ));
		testEnemy->createCollisionBox(world);
		skeletonsVector.push_back(testEnemy);
		*/
		
		//test Medusa
		//std::shared_ptr<Medusa> testEnemy = std::shared_ptr<Medusa>(new Medusa(mazeGenerator.startX - 10, mazeGenerator.startY));
		//testEnemy->createCollisionBox(world);
		//medusaVector.push_back(testEnemy);

	//	std::shared_ptr<Medusa> testEnemy2 = std::shared_ptr<Medusa>(new Medusa(2000, 2000));
	//	testEnemy2->createCollisionBox(world);
	//	medusaVector.push_back(testEnemy2);

		//test minotaur
		std::shared_ptr<Minotaur> testEnemy = std::shared_ptr<Minotaur>(new Minotaur(mazeGenerator.startX - 10, mazeGenerator.startY));
		testEnemy->createCollisionBox(world);
		minotaurVector.push_back(testEnemy);


		//test Traps
		//std::shared_ptr<SpikeTrap> testSpikeTrap = std::shared_ptr<SpikeTrap>(new SpikeTrap(playerCharacter->xPosition , playerCharacter->yPosition + 9));
		//testSpikeTrap->createBox2D(world);
		//trapVector.push_back(testSpikeTrap);

		//std::shared_ptr<ArrowTrap> testArrowTrap = std::shared_ptr<ArrowTrap>(new ArrowTrap(playerCharacter->xPosition , playerCharacter->yPosition + 10));
		//testArrowTrap->createBox2D(world);
		//trapVector.push_back(testArrowTrap);

		std::shared_ptr<IdolBoudlerTrap> testIdolTrap = std::shared_ptr<IdolBoudlerTrap>(new IdolBoudlerTrap(playerCharacter->xPosition , playerCharacter->yPosition + 10));
		testIdolTrap->createBox2D(world);
		trapVector.push_back(testIdolTrap);

		//GUI CREATION//////////////////////////////////////////
		if (gui->getWidgets().size() <= 0) {//if the gui is empty 
		//create the gui for the level and add it to the GUI holder. do this here instead of update becasue we don't want to keep cvreating them over and over again
			treasureUI = tgui::TextBox::create();
			treasureUI->setPosition(0, 0); // set the textBox to be positioned at 0,0
			treasureUI->setSize(200, 25); // set the size
			treasureUI->setTextSize(16); // set the font size
			treasureUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
			treasureUI->getRenderer()->setBorderColor(sf::Color::Blue);//border colour

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
			torchFuelUI->setPosition(425, 0); // set the textBox to be positioned at 400,0
			torchFuelUI->setSize(600, 12.5); // set the size
			torchFuelUI->setTextSize(10); // set the font size
			torchFuelUI->getRenderer()->setBackgroundColor(sf::Color::Transparent); // set colour
			torchFuelUI->getRenderer()->setBorderColor(sf::Color::Blue);//border colour

			
			//two below are awakward. normal rectangles, so get drawn normally to the window, rather than to the view like the gui stuff using TGUI. have to psoition them relative to the player, and make them very small to fit the zoomed in view
			//the red backgroundportion
			torchFuelUIBackgroundRed.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
			torchFuelUIBackgroundRed.setSize(sf::Vector2f(37.5f,1.4f));
			torchFuelUIBackgroundRed.setFillColor(sf::Color::Red);

			//the white backgroundportion - create two background portions so that this one is left behind as the red one updates and shrinks with the current fuel
			torchFuelUIBackgroundWhite.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
			torchFuelUIBackgroundWhite.setSize(sf::Vector2f(37.5f, 1.4f));
			torchFuelUIBackgroundWhite.setFillColor(sf::Color::White);

			gui->add(torchFuelUI);

			//torch image
			torchImage = tgui::Picture::create(textureLoader.torchTexture);

			torchImage->setPosition(400, 0); // set the textBox to be positioned at 400,0
			torchImage->setSize(25, 12.5); // set the size

			gui->add(torchImage);

			//SHIELD UI
			ShieldUI = tgui::TextBox::create();
			ShieldUI->setPosition(425, 12.5); // set the textBox to be positioned at 400,0
			ShieldUI->setSize(600, 12.5); // set the size
			ShieldUI->setTextSize(10); // set the font size
			ShieldUI->getRenderer()->setBackgroundColor(sf::Color::Transparent); // set colour
			ShieldUI->getRenderer()->setBorderColor(sf::Color::Blue);//border colour

			gui->add(ShieldUI);

			//the blue backgroundportion
			shieldUIBackgroundBlue.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 48.6f);
			shieldUIBackgroundBlue.setSize(sf::Vector2f(37.5f, 1.4f));
			shieldUIBackgroundBlue.setFillColor(sf::Color::Cyan);

			//the white backgroundportion - create two background portions so that this one is left behind as the red one updates and shrinks with the current fuel
			shieldUIBackgroundWhite.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 48.6f);
			shieldUIBackgroundWhite.setSize(sf::Vector2f(37.5f, 1.4f));
			shieldUIBackgroundWhite.setFillColor(sf::Color::White);

			//shield image
			shieldImage = tgui::Picture::create(textureLoader.shieldIcon);
			
			shieldImage->setPosition(400, 12.5); // set the textBox to be positioned at 400,0
			shieldImage->setSize(25, 12.5); // set the size
			
			gui->add(shieldImage);
			

			
			//currentitem Ui Creation
			currentItemUI = tgui::TextBox::create();
			currentItemUI->setPosition(1025, 0); // set the textBox to be positioned at 400,0
			currentItemUI->setSize(50, 25); // set the size
			currentItemUI->setTextSize(16); // set the font size
			currentItemUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
			currentItemUI->getRenderer()->setBorderColor(sf::Color::Blue);//border colour

			gui->add(currentItemUI);

			//set the inital icon
			string item = playerCharacter->AquiredItems[playerCharacter->currentItem]->itemName; //get the name of the item
																								 //set the background texture based on item name
			if (item == "Rope") {

			}
			else if (item == "FlameCloak") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.flameCloakGUITexture); //set the background texture
			}
			else if (item == "BombItem") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.bombGUITexture); //set the background texture
			}
			else if (item == "HermesBoots") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.hermesBootGUITexture); //set the background texture
			}
			else if (item == "HermesHelm") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.hermesHelmIcon); //set the background texture
			}
			else if (item == "GoldenFleece") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.goldenFleece); //set the background texture
			}
			else if (item == "MedusaHead") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.medusaHeadIcon); //set the background texture
			}
		}

		std::shared_ptr<ExitCell> exit;
		exit = std::shared_ptr<ExitCell>(new ExitCell(mazeGenerator.endX, mazeGenerator.endY));
		mazeGenerator.cellsVector.push_back(exit);


		//create treasure chests
		for (int i = 0; i < 3;) {
			int chestX = rand() % 1599 + 1;
			int chestY = rand() % 899 + 1; //create X and Y postitons in the range of positions they can be 

			if (mazeGenerator.maze[chestX / 10][chestY / 10] == 0) {// check the randomised position against the maze to see if it's a floor (don't want  them to spawn in walls) (divide by 10 to change from screenposition to array position)
				treasureChestVector.push_back(std::shared_ptr<TreasureChest>(new TreasureChest(playerCharacter->xPosition + 10 + (i *2), playerCharacter->yPosition + 5))); //for testing, push a single chest to the back
				treasureChestVector.back()->createBody(world); //create the body
				i++; // progress i. progress i here so that we're guarenteed 3 chests
			}

		}
		treasureChestVector.push_back(std::shared_ptr<TreasureChest>( new TreasureChest(playerCharacter->xPosition + 5 , playerCharacter->yPosition +5 ))); //for testing, push a single chest to the back
		treasureChestVector.back()->createBody(world); //create the body

		
		
		treasureVector.push_back(std::shared_ptr<Treasure>(new Treasure(playerCharacter->xPosition + 10.f, playerCharacter->yPosition)));
		treasureVector.back()->CreateBody(world);

		//create the minimap
		gameMiniMap = new MiniMap(mazeGenerator.cellsVector, skeletonsVector, medusaVector); //create a new minimap when we load a new level

		playerCharacter->createCollisionBox(world);
		currentState = inGameState;// set the current state to be ingame state so that we can start updating the game
	}
}



void LevelManager::DeleteCurrentLevel(b2World &world)
{
	world.DestroyBody(playerCharacter->dynamicBody); // destroy the player character body
	//playerCharacter->dynamicBody = nullptr; // set body null
	//playerCharacter->currentTorchFuel = playerCharacter->maxTorchFuel; //reset the amount of fuel the player has for the torch

	//clear the vectors
	for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
		if (mazeGenerator.cellsVector[i]->cellBody != nullptr) {
			world.DestroyBody(mazeGenerator.cellsVector[i]->cellBody); //destroy all the cell bodies
		}
	}
	mazeGenerator.cellsVector.clear(); // clear the cells vector so that we can create the new maze

	for (int i = 0; i < skeletonsVector.size(); i++) {
		world.DestroyBody(skeletonsVector[i]->dynamicBody); //destroy all the skeleton bodies
	}
	skeletonsVector.clear(); //clear the skeletons vector

	for (int i = 0; i < medusaVector.size(); i++) {
		world.DestroyBody(medusaVector[i]->dynamicBody); //destroy all the medusa bodies
	}
	medusaVector.clear();//clear the medusa vector

	for (int i = 0; i < griffinVector.size(); i++) {
		world.DestroyBody(griffinVector[i]->dynamicBody); //destroy all the griffin bodies
	}
	griffinVector.clear();//clear the griffin vector

	for (int i = 0; i < ghostVector.size(); i++) {
		world.DestroyBody(ghostVector[i]->dynamicBody); //destroy all the ghjost bodies
	}
	ghostVector.clear();//clear the Ghost vector

	for (int i = 0; i < treasureVector.size(); i++) {
		world.DestroyBody(treasureVector[i]->Body); //destroy all the griffin bodies
	}
	treasureVector.clear();//clear the Treasure vector

	for (int i = 0; i < treasureChestVector.size(); i++) {
		world.DestroyBody(treasureChestVector[i]->chestBody); //destroy all the treasureChest bodies
	}
	treasureChestVector.clear(); //clear treasure chests

	for (int i = 0; i < trapVector.size(); i++) {
		world.DestroyBody(trapVector[i]->Box2DBody); //destroy all the trap bodies
	}
	trapVector.clear(); //clear traps

	gui->removeAllWidgets(); //remove all the gui

	
	if (escapeRun == false) { // if not on the escape run - do the normal load level
		currentState = loadLevelState; // transition to load level state to load the next level
	}
	else if (escapeRun == true) { //otherwise
		currentState = loadEscapeRunLevelState; //transition to the escape run load level state instead
	}
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
	//update any ghosts
	for (int i = 0; i < ghostVector.size(); i++) {
		ghostVector[i]->update();
	}
	//update any griffins
	for (int i = 0; i < griffinVector.size(); i++) {
		griffinVector[i]->update(playerCharacter);
	}
	//update the minotaur once it spawns
	for (int i = 0; i < minotaurVector.size(); i++) {
		minotaurVector[i]->update(playerCharacter, &mazeGenerator);
	}

	//update any treasure chests
	for (int i = 0; i < treasureChestVector.size(); i++) {
		treasureChestVector[i]->update();
	}

	//update the traps if they need it
	for (int i = 0; i < trapVector.size(); i++) {
		trapVector[i]->update();
	}
	//update treasure
	for (int i = 0; i < treasureVector.size(); i++) {
		treasureVector[i]->update();
	}


	//////////////////////GUI UPDATE//////////////////////////////////////////////////////////////////

	//treasure UI
	treasureUI->setText("Treasure: " + to_string(playerCharacter->treasure));//set the treasure ui string to be the amount of treasure the player has

	//TorchFuel UI
	//get the two values and store them in an int to remove the extra .0000000's from the end of the number for the text
	//still use the float value for the bar calculation for accuracy
	int fuel = playerCharacter->currentTorchFuel;
	int maxFuel = playerCharacter->maxTorchFuel;
	torchFuelUI->setText("Fuel Remaining: " + to_string(fuel) + " / " + to_string(maxFuel)); //set the text for the fuel UI to display the current and max fuel. 
	float redBarSize = (playerCharacter->currentTorchFuel / playerCharacter->maxTorchFuel) * 37.5f; // work out the new size of the red portion of the UI background by getting the fraction and multiplying it by the max width
	
	torchFuelUIBackgroundRed.setSize(sf::Vector2f(redBarSize, 1.4f));
	//have to keep updating the position based on the player's position becasue opf how the view follows the player, and these are just normal rectnagles, so don't get drawn staright to the view
	torchFuelUIBackgroundRed.setPosition(playerCharacter->xPosition - 23.4, playerCharacter->yPosition - 50);
	torchFuelUIBackgroundWhite.setPosition(playerCharacter->xPosition - 23.4, playerCharacter->yPosition - 50);

	//same as torchUI update but for the shield
	//get the two values and store them in an int to remove the extra .0000000's from the end of the number for the text
	//still use the float value for the bar calculation for accuracy
	int energy = playerCharacter->shieldEnergy;
	int maxEnergy = playerCharacter->shieldEnergyMax;
	ShieldUI->setText("Energy Remaining: " + to_string(energy) + " / " + to_string(maxEnergy)); //set the text for the Shield UI to display the current and max energy. 
	float blueBarSize = (playerCharacter->shieldEnergy / playerCharacter->shieldEnergyMax) * 37.5f; // work out the new size of the Blue portion of the UI background by getting the fraction and multiplying it by the max width

	shieldUIBackgroundBlue.setSize(sf::Vector2f(blueBarSize, 1.4f));
	//have to keep updating the position based on the player's position becasue opf how the view follows the player, and these are just normal rectnagles, so don't get drawn staright to the view
	shieldUIBackgroundBlue.setPosition(playerCharacter->xPosition - 23.4, playerCharacter->yPosition - 48.6f);
	shieldUIBackgroundWhite.setPosition(playerCharacter->xPosition - 23.4, playerCharacter->yPosition - 48.6f);

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
	

	//update the current item GUI
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { 
		if (playerCharacter->itemsOnCooldown == false) { // check for the same button press and condition as player so it only changes when the player changes the current item
			string item = playerCharacter->AquiredItems[playerCharacter->currentItem]->itemName; //get the name of the item
			//set the background texture based on item name
			if (item == "Rope") {

			}
			else if (item == "FlameCloak") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.flameCloakGUITexture); //set the background texture
			}
			else if (item == "BombItem") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.bombGUITexture); //set the background texture
			}
			else if (item == "HermesBoots") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.hermesBootGUITexture); //set the background texture
			}
			else if (item == "HermesHelm") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.hermesHelmIcon); //set the background texture
			}
			else if (item == "GoldenFleece") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.goldenFleece); //set the background texture
			}
			else if (item == "MedusaHead") {
				currentItemUI->getRenderer()->setTextureBackground(textureLoader.medusaHeadIcon); //set the background texture
			}
		}
	}
	if (playerCharacter->itemsOnCooldown == true) { //if items on cooldown, draw the text to be the cooldown timer
		int cooldownTimer = playerCharacter->AquiredItems[playerCharacter->currentItem]->coolDownTimer - playerCharacter->CurrentItemClock->getElapsedTime().asSeconds();
		currentItemUI->setText(to_string(cooldownTimer)); //get the reaminging cooldown time by get the cooldown duration form the item and minusing the elapsed time from the player's cooldown timer
	}
	else (currentItemUI->setText("")); //set blank text if not on cooldown
	

	//DESPAWN / DESTROY SECTION
	//use the despawn Manger to check if we need to remove anything and do so if needed
	despawnManger.despawnTreasurevector(treasureVector, World);
	despawnManger.despawnMedusa(medusaVector, World);
	despawnManger.despawnSkeleton(skeletonsVector, World);
	despawnManger.despawnGhost(ghostVector, World);
	despawnManger.despawnGriffin(griffinVector, World);
	despawnManger.despawnWalls(mazeGenerator.cellsVector, World);

	/////////////////////////DRAW///////////////////////////////////////////////////
	

	//set the window to use the cameraview
//	window->setView(playerView->cameraView);

	//draw all the maze cells
	for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
		if (mazeGenerator.cellsVector[i]->rectangle.getPosition().x >= playerCharacter->xPosition && mazeGenerator.cellsVector[i]->rectangle.getPosition().x <= playerCharacter->xPosition + 50.f ||
			mazeGenerator.cellsVector[i]->rectangle.getPosition().x <= playerCharacter->xPosition && mazeGenerator.cellsVector[i]->rectangle.getPosition().x >= playerCharacter->xPosition - 60.f) { //if the cell is within the camera view
			if (mazeGenerator.cellsVector[i]->rectangle.getPosition().y >= playerCharacter->yPosition && mazeGenerator.cellsVector[i]->rectangle.getPosition().y <= playerCharacter->yPosition + 50.f ||
				mazeGenerator.cellsVector[i]->rectangle.getPosition().y <= playerCharacter->yPosition && mazeGenerator.cellsVector[i]->rectangle.getPosition().y >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw the cell if it's in our camera view

				window->draw(mazeGenerator.cellsVector[i]->rectangle);
			}
		}
	}

	//draw the skeletons
	for (int i = 0; i < skeletonsVector.size(); i++) {
		if (skeletonsVector[i]->xPosition >= playerCharacter->xPosition && skeletonsVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			skeletonsVector[i]->xPosition <= playerCharacter->xPosition && skeletonsVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the skeleton is within the camera view
			if (skeletonsVector[i]->yPosition >= playerCharacter->yPosition && skeletonsVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				skeletonsVector[i]->yPosition <= playerCharacter->yPosition && skeletonsVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition

				window->draw(skeletonsVector[i]->rectangle);
			}
		}
	}
	//draw medusas
	for (int i = 0; i < medusaVector.size(); i++) {
		if (medusaVector[i]->xPosition >= playerCharacter->xPosition && medusaVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			medusaVector[i]->xPosition <= playerCharacter->xPosition && medusaVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the medusa is within the camera view
			if (medusaVector[i]->yPosition >= playerCharacter->yPosition && medusaVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				medusaVector[i]->yPosition <= playerCharacter->yPosition && medusaVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(medusaVector[i]->rectangle);
			}
		}
	}
	//draw ghosts
	for (int i = 0; i < ghostVector.size(); i++) {
		if (ghostVector[i]->xPosition >= playerCharacter->xPosition && ghostVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			ghostVector[i]->xPosition <= playerCharacter->xPosition && ghostVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the medusa is within the camera view
			if (ghostVector[i]->yPosition >= playerCharacter->yPosition && ghostVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				ghostVector[i]->yPosition <= playerCharacter->yPosition && ghostVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(ghostVector[i]->rectangle);
			}
		}
	}
	//draw griffins
	for (int i = 0; i < griffinVector.size(); i++) {
		if (griffinVector[i]->xPosition >= playerCharacter->xPosition && griffinVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			griffinVector[i]->xPosition <= playerCharacter->xPosition && griffinVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the medusa is within the camera view
			if (griffinVector[i]->yPosition >= playerCharacter->yPosition && griffinVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				griffinVector[i]->yPosition <= playerCharacter->yPosition && griffinVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(griffinVector[i]->rectangle);
			}
		}
	}
	//darw all the treasure
	for (int i = 0; i < treasureVector.size(); i++) {
		if (treasureVector[i]->position.x >= playerCharacter->xPosition && treasureVector[i]->position.x <= playerCharacter->xPosition + 50.f ||
			treasureVector[i]->position.x <= playerCharacter->xPosition && treasureVector[i]->position.x >= playerCharacter->xPosition - 60.f) { //if the treasure is within the camera view
			if (treasureVector[i]->position.y >= playerCharacter->yPosition && treasureVector[i]->position.y <= playerCharacter->yPosition + 50.f ||
				treasureVector[i]->position.y <= playerCharacter->yPosition && treasureVector[i]->position.y >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(treasureVector[i]->rectangle);
			}
		}
	}

	//draw the treasureChests
	for (int i = 0; i < treasureChestVector.size(); i++) {
		if (treasureChestVector[i]->xPosition >= playerCharacter->xPosition && treasureChestVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			treasureChestVector[i]->xPosition <= playerCharacter->xPosition && treasureChestVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the medusa is within the camera view
			if (treasureChestVector[i]->yPosition >= playerCharacter->yPosition && treasureChestVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				treasureChestVector[i]->yPosition <= playerCharacter->yPosition && treasureChestVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(treasureChestVector[i]->rectangle); //darw the chest
				if (treasureChestVector[i]->alreadyOpened == true) { //once opened
					window->draw(treasureChestVector[i]->itemSprite); //draw the treaureChest item sprite
				}
			}
		}
	}

	//draw traps
	for (int i = 0; i < trapVector.size(); i++) {
		if (trapVector[i]->xPosition >= playerCharacter->xPosition && trapVector[i]->xPosition <= playerCharacter->xPosition + 50.f ||
			trapVector[i]->xPosition <= playerCharacter->xPosition && trapVector[i]->xPosition >= playerCharacter->xPosition - 60.f) { //if the trap is within the camera view
			if (trapVector[i]->yPosition >= playerCharacter->yPosition && trapVector[i]->yPosition <= playerCharacter->yPosition + 50.f ||
				trapVector[i]->yPosition <= playerCharacter->yPosition && trapVector[i]->yPosition >= playerCharacter->yPosition - 60.f) { //same check as above but for yposition - draw culling. only draw things that are within our camera view to save performance

				window->draw(trapVector[i]->rectangle); //darw the trap
				
			}
		}
	}

	//for testing
	//if L is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		/*for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) { //search vector
			if (mazeGenerator.cellsVector[i]->cellType == "Exit") { // find the exit cell
				mazeGenerator.cellsVector[i]->ExitReached = true;
			}
		}*/
		currentState = deleteLevelState;
	}
	
	//check for reaching the exit
	for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) { //search vector
		if (mazeGenerator.cellsVector[i]->cellType == "Exit") { // find the exit cell
			// if player is intersecting with exit 
			if (playerCharacter->rectangle.getGlobalBounds().left + playerCharacter->rectangle.getGlobalBounds().width >= mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().left
				&& playerCharacter->rectangle.getGlobalBounds().left <= mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().left + mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().width
				&& playerCharacter->rectangle.getGlobalBounds().top + playerCharacter->rectangle.getGlobalBounds().height >= mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().top &&
				playerCharacter->rectangle.getGlobalBounds().top <= mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().top + mazeGenerator.cellsVector[i]->rectangle.getGlobalBounds().height) {

				//completed level so go to deleteLevel
				currentState = deleteLevelState;
			}
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
	window->draw(playerCharacter->rectangle);
	if (playerCharacter->isStone == true) {
		window->draw(playerCharacter->petrifyOverlay); //if we are petrified, draw the petrify overlay( translucent brown) over the top of the playersprite
	}
	window->draw(playerCharacter->shieldCircle);
	window->draw(playerCharacter->torch->torchSprite);
	
	if (playerCharacter->activeItem != nullptr) { // so loong as active item isn't null
		window->draw(playerCharacter->activeItem->rectangle);//draw it
	}
	//set the window to the minimap view so we can tell the minimap what to draw
//	window->setView(gameMiniMap->minimapView);

	//draw all the minimap stuff and update it 
//	gameMiniMap->MiniMapUpdate(window, mazeGenerator.cellsVector, skeletonsVector, medusaVector, playerCharacter);
	

	//set the window back to the normal view
	window->setView(playerView->cameraView);

}


void LevelManager::upgradesMenu()
{
	playerCharacter->xPosition = 20000; //move the player off screen. don';t destory so we still have access to treasure, upgrades ,etc
	
	//create the upgrade menu buttons
	tgui::Button::Ptr UpgradeItemDamage = tgui::Button::create(); // create a button on the menu
	UpgradeItemDamage->setPosition(200, 200); // set the position of the button
	UpgradeItemDamage->setSize(800, 50); // set the start button size
	UpgradeItemDamage->setText("Upgrade Item Damage"); // set text
	UpgradeItemDamage->setTextSize(40); // set size of the text

	gui->add(UpgradeItemDamage); //add the start button to the gui so that it can be drawn and managed.

	UpgradeItemDamage->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

	//next button
	tgui::Button::Ptr UpgradeMaxHealth = tgui::Button::create(); // create a button on the menu
	UpgradeMaxHealth->setPosition(200, 300); // set the position of the button
	UpgradeMaxHealth->setSize(800, 50); // set the start button size
	UpgradeMaxHealth->setText("Upgrade Max Health"); // set text
	UpgradeMaxHealth->setTextSize(40); // set size of the text

	gui->add(UpgradeMaxHealth); //add the start button to the gui so that it can be drawn and managed.

	UpgradeMaxHealth->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button


	//next Button 
	tgui::Button::Ptr PlayerAddHealth = tgui::Button::create(); // create a button on the menu
	PlayerAddHealth->setPosition(200, 400); // set the position of the button
	PlayerAddHealth->setSize(800, 50); // set the start button size
	PlayerAddHealth->setText("Recover Health"); // set text
	PlayerAddHealth->setTextSize(40); // set size of the text

	gui->add(PlayerAddHealth); //add the start button to the gui so that it can be drawn and managed.

	PlayerAddHealth->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button
	
	//next button
	tgui::Button::Ptr ShieldEnergyRefill = tgui::Button::create(); // create a button on the menu
	ShieldEnergyRefill->setPosition(200, 500); // set the position of the button
	ShieldEnergyRefill->setSize(800, 50); // set the start button size
	ShieldEnergyRefill->setText("Shield Energy Refill"); // set text
	ShieldEnergyRefill->setTextSize(40); // set size of the text

	gui->add(ShieldEnergyRefill); //add the start button to the gui so that it can be drawn and managed.

	ShieldEnergyRefill->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

	//next button
	tgui::Button::Ptr ShieldMaxEnergyUpgrade = tgui::Button::create(); // create a button on the menu
	ShieldMaxEnergyUpgrade->setPosition(200, 600); // set the position of the button
	ShieldMaxEnergyUpgrade->setSize(800, 50); // set the start button size
	ShieldMaxEnergyUpgrade->setText("Shield Max Energy Upgrade"); // set text
	ShieldMaxEnergyUpgrade->setTextSize(40); // set size of the text

	gui->add(ShieldMaxEnergyUpgrade); //add the start button to the gui so that it can be drawn and managed.

	ShieldMaxEnergyUpgrade->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button


	//next button
	tgui::Button::Ptr TorchRefill = tgui::Button::create(); // create a button on the menu
	TorchRefill->setPosition(200, 700); // set the position of the button
	TorchRefill->setSize(800, 50); // set the start button size
	TorchRefill->setText("Torch Refill"); // set text
	TorchRefill->setTextSize(40); // set size of the text

	gui->add(TorchRefill); //add the start button to the gui so that it can be drawn and managed.

	TorchRefill->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

	//setup the cost boxes next to the Buttons and our player GUI so we know what we have
	//our current treasure textBox
	treasureUI = tgui::TextBox::create();
	treasureUI->setPosition(0, 0); // set the textBox to be positioned at 0,0
	treasureUI->setSize(200, 25); // set the size
	treasureUI->setTextSize(20); // set the font size
	treasureUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	treasureUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(treasureUI); // add the treasure UI to the gui

	///////HEALTH UI///////////////
	healthUI = tgui::TextBox::create();
	healthUI->setPosition(200, 0);
	healthUI->setSize(200, 25);
	//healthUI->getRenderer()->setTextureBackground(textureLoader.healthBarTexture2);

	gui->add(healthUI);

	///////////////////////FUEL UI////////////////////////////////////
	//the textbox portion
	torchFuelUI = tgui::TextBox::create();
	torchFuelUI->setPosition(400, 0); // set the textBox to be positioned at 400,0
	torchFuelUI->setSize(600, 25); // set the size
	torchFuelUI->setTextSize(16); // set the font size
	torchFuelUI->getRenderer()->setBackgroundColor(sf::Color::Transparent); // set colour
	torchFuelUI->getRenderer()->setBorderColor(sf::Color::Green);//border colour

	gui->add(torchFuelUI);

	//Cost box
	ItemDamageCostUI = tgui::TextBox::create();
	ItemDamageCostUI->setPosition(1100, 200); // set the textBox to be positioned at 0,0
	ItemDamageCostUI->setSize(200, 50); // set the size
	ItemDamageCostUI->setTextSize(20); // set the font size
	ItemDamageCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	ItemDamageCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(ItemDamageCostUI); // add the treasure UI to the gui

	//next Cost Box
	MaxHealthCostUI = tgui::TextBox::create();
	MaxHealthCostUI->setPosition(1100, 300); // set the textBox to be positioned at 0,0
	MaxHealthCostUI->setSize(200, 50); // set the size
	MaxHealthCostUI->setTextSize(20); // set the font size
	MaxHealthCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	MaxHealthCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(MaxHealthCostUI); // add the treasure UI to the gui

	//next cost box
	AddHealthCostUI = tgui::TextBox::create();
	AddHealthCostUI->setPosition(1100, 400); // set the textBox to be positioned at 0,0
	AddHealthCostUI->setSize(200, 50); // set the size
	AddHealthCostUI->setTextSize(20); // set the font size
	AddHealthCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	AddHealthCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(AddHealthCostUI); // add the treasure UI to the gui

	//next cost box
	EnergyRefillCostUI = tgui::TextBox::create();
	EnergyRefillCostUI->setPosition(1100, 500); // set the textBox to be positioned at 0,0
	EnergyRefillCostUI->setSize(200, 50); // set the size
	EnergyRefillCostUI->setTextSize(20); // set the font size
	EnergyRefillCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	EnergyRefillCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(EnergyRefillCostUI); // add the treasure UI to the gui

	//next cost Box
	EnergyMaxUpgradeCostUI = tgui::TextBox::create();
	EnergyMaxUpgradeCostUI->setPosition(1100, 600); // set the textBox to be positioned at 0,0
	EnergyMaxUpgradeCostUI->setSize(200, 50); // set the size
	EnergyMaxUpgradeCostUI->setTextSize(20); // set the font size
	EnergyMaxUpgradeCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	EnergyMaxUpgradeCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(EnergyMaxUpgradeCostUI); // add the treasure UI to the gui

	//next cost Box
	TorchRefillCostUI = tgui::TextBox::create();
	TorchRefillCostUI->setPosition(1100, 700); // set the textBox to be positioned at 0,0
	TorchRefillCostUI->setSize(200, 50); // set the size
	TorchRefillCostUI->setTextSize(20); // set the font size
	TorchRefillCostUI->getRenderer()->setBackgroundColor(sf::Color::White); // set colour
	TorchRefillCostUI->getRenderer()->setBorderColor(sf::Color::Black);//border colour

	gui->add(TorchRefillCostUI); // add the treasure UI to the gui


	//create two buttons, one for advance and one for going back
	tgui::Button::Ptr CarryOnButton = tgui::Button::create(); // create a button on the menu
	CarryOnButton->setPosition(400, 800); // set the position of the button
	CarryOnButton->setSize(400, 50); // set the start button size
	CarryOnButton->setText("Continue Game"); // set text
	CarryOnButton->setTextSize(40); // set size of the text

	gui->add(CarryOnButton); //add the start button to the gui so that it can be drawn and managed.

	CarryOnButton->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button

	//second button
	tgui::Button::Ptr BeginBackwardsRun = tgui::Button::create(); // create a button on the menu
	BeginBackwardsRun->setPosition(900, 800); // set the position of the button
	BeginBackwardsRun->setSize(400, 50); // set the start button size
	BeginBackwardsRun->setText("Begin Escape Run"); // set text
	BeginBackwardsRun->setTextSize(40); // set size of the text

	gui->add(BeginBackwardsRun); //add the start button to the gui so that it can be drawn and managed.

	BeginBackwardsRun->connect(std::string("pressed"), &LevelManager::SignalManager, this); // connect the button. tell it to work on press, call the signal manager function and sets the msg passed to be the text on the button



	
	currentState = upgradeIdleState; //transition to idle state so that we don't keep creating the buttons over and over again while we use the menu
}

void LevelManager::upgradesMenuIdle()
{
	//set the text for all of the cost boxes and our treasure text box in the "idle" state for the upgrade menu so that it keeps getting updated ( this function will be run until we leave the upgrade menu)
	treasureUI->setText("Treasure: " + to_string(playerCharacter->treasure)); //set the treasure text to be our current treasure
	ItemDamageCostUI->setText("Cost: " + to_string(itemDamageUpgradeCost* itemDamageUpgradeMultiplier)); //set the text to be "cost : the result of the cost * the multiplier"
	MaxHealthCostUI->setText("Cost: " + to_string(maxHealthUpgradeCost* maxHealthUpgradeMultiplier));
	AddHealthCostUI->setText("Cost: 2000"); //has  a set cost, so don't need to convert the int to a string and can just set the number in the string
	EnergyRefillCostUI->setText("Cost: 2000");
	EnergyMaxUpgradeCostUI->setText("Cost: " + to_string(energyMaxUpgradeCost* energyMaxUpgradeMultiplier));
	TorchRefillCostUI->setText("Cost: 500");
	//TorchFuel UI
	torchFuelUI->setText("Fuel Remaining: " + to_string(playerCharacter->currentTorchFuel) + " / " + to_string(playerCharacter->maxTorchFuel)); //set the text for the fuel UI to display the current and max fuel. 

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
}

void LevelManager::DeleteUpgradeMenu()
{
	//delete the upgrade menu GUI
	gui->removeAllWidgets();
	if (escapeRun == false) { // if not on escape run
		currentState = loadLevelState; //transition to the next level
	}
	else if (escapeRun == true) {
		currentState = loadEscapeRunLevelState;// escape run level load
	}
}


//reload the saved levels for a backward run
void LevelManager::loadBackwardsLevel(b2World & world)
{
	
	escapeRun = true; //set true so we know we're on the escape run
	levelCounter--; //go backwards in the level counter
	if (levelCounter == 0) { // once player through all the levels
		currentState = menuCreate; // just go back to main menu
		escapeRun = false; // going back to the menu so no longer on the escape run and want this to be set false if the player starts another run
	}
	else if (levelCounter % 3 == 0) {//if it would be a upgrade menu level usually
		levelCounter--; //reduce level counter by one more - don't want upgrade stops on the escape run 
	}
	else {
		string levelString = "Assets/Level" + to_string(levelCounter) + ".txt"; //get the directory of the level to go back to using the counter
		int line; //the current value being read
		ifstream levelFile;
		//counters to work out the x and y postions - need to use these becasue the normal mazegeneration increments through a maze double array and uses the position in the array for x and y positions
		//however cause we aren't making a new maze and loading one from a file, we can't do this. so use these counters that increment as we load each value from the file instead.
		int xCounter = 0;
		int yCounter = 0;
		
		
		//if (levelFile.is_open()) { // open the file
		levelFile.open(levelString);
		if (!levelFile) {
			cerr << "Unable to open file datafile.txt";
			//exit(1);   // call system to stop
		}

			while (levelFile >> line) //put the values from the file into the line holder
			{
				//if maze cell is 1 create wall
				if (line == 1) {
					printf("1");
					mazeGenerator.cellsVector.push_back(std::shared_ptr<WallCell>(new WallCell(xCounter * 10, yCounter * 10, world)));

				}
				else if (line == 2) { // if 4, the cell is a fake wall created during the random extra walls.
					printf("2");
					mazeGenerator.cellsVector.push_back(std::shared_ptr<FakeWallCell>(new FakeWallCell(xCounter * 10, yCounter * 10, world)));
				}
				else if (line == 0) {
					printf("0"); // create floorcell
					mazeGenerator.cellsVector.push_back(std::shared_ptr<FloorCell>(new FloorCell(xCounter * 10, yCounter * 10)));
					

				}
				else if (line == 4) {
					printf("4"); //exitcell
								 //cellsVector.push_back(new ExitCell(x * 10, y * 10));
				}
				else if (line == 3) { // if 4, the cell is completely surronded by walls, so create a nonbodyWallcell
					printf("3");
					//was going to create these non body wall cells originally to reduce the amount of lag, however, hoaving this number of images to render and draw also caused lag (but less than having them all be bodies). so instead when it's a 3 just don't draw anything at all and i will add a background. Massivly improves performance
					//cellsVector.push_back(std::shared_ptr<NonBodyWallCell>(new NonBodyWallCell(x * 10, y * 10)));
				}
				else if (line == 5) { // 5 is still floor, but the p[layer's spawn position so special
					printf("5"); // create floorcell
					mazeGenerator.cellsVector.push_back(std::shared_ptr<FloorCell>(new FloorCell(xCounter * 10, yCounter * 10)));
					
					playerCharacter->xPosition = xCounter * 10;
					playerCharacter->yPosition = yCounter * 10;
				}

				yCounter++; // update the position

				if (yCounter >= 60) { // once we've reached the end of the line // the highest y position - needs to be the same as the maze size in mazegenertor otherwise the loaded levels will be broken
					yCounter = 0; // reset back to 1st y position
					xCounter++; //increment xPosition
				}
			}

			//levelFile.close(); //close once loaded the level
		//}



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
			torchFuelUIBackgroundRed.setSize(sf::Vector2f(37.5f, 2.8f));
			torchFuelUIBackgroundRed.setFillColor(sf::Color::Red);

			//the white backgroundportion - create two background portions so that this one is left behind as the red one updates and shrinks with the current fuel
			torchFuelUIBackgroundWhite.setPosition(playerCharacter->xPosition - 25, playerCharacter->yPosition - 50);
			torchFuelUIBackgroundWhite.setSize(sf::Vector2f(37.5f, 2.8f));
			torchFuelUIBackgroundWhite.setFillColor(sf::Color::White);

			gui->add(torchFuelUI);

		}

		//create the minimap
		gameMiniMap = new MiniMap(mazeGenerator.cellsVector, skeletonsVector, medusaVector); //create a new minimap when we load a new level
		int playerX = rand() % 1599 + 1;
		
		
		playerCharacter->createCollisionBox(world);
		currentState = inGameState;// set the current state to be ingame state so that we can start updating the game
	}
	


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
	else if (msg == "Shield Energy Refill") {
		if (playerCharacter->treasure >= 2000) { // if the player has more treasure than the cost
			if (playerCharacter->shieldEnergy < playerCharacter->shieldEnergyMax) { // if the current energy is less than max energy we can add an extra health point
				playerCharacter->shieldEnergy = playerCharacter->shieldEnergyMax; // add energy back to player
				playerCharacter->treasure = playerCharacter->treasure - 2000; //remove the treasure
																			  //no multiplier for this one. just high initial cost
			}
		}
	}
	else if (msg == "Shield Max Energy Upgrade") {
		if (playerCharacter->treasure >= energyMaxUpgradeCost * energyMaxUpgradeMultiplier) {
			playerCharacter->shieldEnergyMax = playerCharacter->shieldEnergyMax + 25.f; // increase max energy
			playerCharacter->shieldEnergy = playerCharacter->shieldEnergyMax; //refill the energy to be nice
			playerCharacter->treasure = playerCharacter->treasure - energyMaxUpgradeCost * energyMaxUpgradeMultiplier; //remove the gold
			energyMaxUpgradeMultiplier = energyMaxUpgradeMultiplier + 0.2f;
		}
	}
	else if (msg == "Torch Refill") {
		if (playerCharacter->treasure >= 500) {
			playerCharacter->currentTorchFuel = playerCharacter->maxTorchFuel; //refill torch fuel
			playerCharacter->treasure - 500; //remove gold
		}
	}
	else if (msg == "Continue Game") {
		currentState = deleteUpgradeMenuState; //transition to the deletion of the upgrade menu to prepare for next level
	}
	else if (msg == "Begin Escape Run") {
		currentState = deleteUpgradeMenuState;//still transition to upgrade menu deletion so that we delete the menus
		escapeRun = true; //but also set this true to change which level gets loaded
	}

}




