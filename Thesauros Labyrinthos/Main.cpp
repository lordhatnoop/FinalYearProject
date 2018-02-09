#include "stdafx.h"
#include "mazeGeneration.h"
#include "PlayerCharacter.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "LevelManager.h"
#include "CollisionListener.h"
using namespace std;



TextureLoader textureLoader; //make global  textureLoader object so it can be used elsewhere becasue of the extern
//declartion in the .h file

 
int main() {
	// Window creation and mangement
	sf::RenderWindow gameWindow(sf::VideoMode(1600, 900), "2DPhysicsPuzzleGame"); // creates a window and sets it's properties (size) */
	gameWindow.setPosition(sf::Vector2i(0, 9.8));
	gameWindow.setFramerateLimit(60); // set the fps limit for the gamewindow to 60 
	
	
	 //Box2D level stuff setup
	b2Vec2 gravity(0, 9.8f * 30);//normal earth gravity = 9.8, have to set it really high becasue everything is so small
	float32 timeStep = 1/ 60.0f;      //the length of time passed to simulate (seconds) */
	int32 velocityIterations = 8.f;   //how strongly to correct velocity */
	int32 positionIterations = 3.f;   //how strongly to correct position */

							
	b2World world(gravity); //create the box2d world and give it gravity

	CollisionListener collisionListener; // create a contactlistener instance

	world.SetContactListener(&collisionListener); // set the world to use our contact listener

	tgui::Gui gui{ gameWindow }; // create a GUI that using tgui and pass it the gamewindow



	LevelManager levelManager(&gameWindow, &gui); // create the levelManager and pass references to the gamewindow and the GUI manager

	 b2GLDraw debugDraw; //setting up a debug draw to display the box2d physics shapes / boxes using GL so that can see the actual physics boxes and not just the sfml graphics placed over top */
	 world.SetDebugDraw(&debugDraw);
														 
	 uint32 flags = 0; //type of drawing mode
														 //types of drawing
														 flags += b2Draw::e_shapeBit;
														 flags += b2Draw::e_jointBit;
														 flags += b2Draw::e_aabbBit;
														 //flags += b2Draw::e_pairBit;
														 //flags += b2Draw::e_centerOfMassBit;

														 debugDraw.SetFlags(flags); //passes flags created above to the debugDraw to be used.
														 

	/*
	//below creates a mazegenerator object and then talls it to call the generatemaze function
	MazeGeneration mazeGenerator;
	mazeGenerator.generateMaze();
	//create a playercharacter object and pass the start position of the maze generation since this will always be a floor tile (good place to start).
	PlayerCharacter playerCharacter(mazeGenerator.startX, mazeGenerator.startY);

	//create a a camera object that will be used for the player's view. pass the player positions so that they can be used to set the center of the camera on the player
	Camera playerView(playerCharacter.xPosition, playerCharacter.yPosition); 
	*/
	
//to be sued for fps
	sf::Clock clock;
	float lastTime = 0;

	while (gameWindow.isOpen()) {


		gameWindow.clear(sf::Color::Black);/*!<  clears the window each frame so that it can be updated and old frames aren't left on screen */
		world.Step(timeStep, velocityIterations, positionIterations); // steps the world physics - like a game loop for the box2d world */

		levelManager.FSM(world);
		
		 //! check all user events
		sf::Event event;
		while (gameWindow.pollEvent(event)) /*!< while the window is checking for events */
		{
			//call the player update function
		//	playerCharacter.update();
			//update the camera and pass the playerposition so camera can follow.
		//	playerView.update(playerCharacter.xPosition, playerCharacter.yPosition);
			
			

			if (event.type == sf::Event::Closed)/*!< User has pressed the close button(red X) */
			{

				gameWindow.close(); /*!<closes the window */
			}

			gui.handleEvent(event); // tell the GUI to handle events and pass the event
		}

		//-----------------------------------------------	//Draw section --------------------------------------------------------------------------------------------------------------------------------
		

		
		//set the window to use the cemraview
		//gameWindow.setView(playerView.cameraView);

		
		/*
		for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
			gameWindow.draw(mazeGenerator.cellsVector[i]->rectangle);
		}
		
		//draw the playersprite and add the shader to it
		gameWindow.draw(playerCharacter.rectangle);
		//gameWindow.draw(playerCharacter.torch->torchSprite);
		//gameWindow.draw(textureLoader.darknessSprite);
		//gameWindow.draw(darknessRectangle);
		//gameWindow.draw(lightCircle,renderState);
		*/

		if (levelManager.getCurrentState() == menuIdle) { // if we're in the menuState, draw the title
			gameWindow.draw(levelManager.title); 
		}

		

		if (levelManager.getCurrentState() == inGameState) {
			//world.DrawDebugData();
			gameWindow.draw(levelManager.torchFuelUIBackgroundWhite);
			gameWindow.draw(levelManager.torchFuelUIBackgroundRed);
		}
	
		gui.draw();// tell the gui to draw. draw after everything else becasue we want it to sit at the top
	 // if in game state draw the two background sections for the torch UI
		
		 //DISPLAY

		 //fps stuff
		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;


		gameWindow.display(); /*!< tells the gamewindow to display everything */
	}


}

