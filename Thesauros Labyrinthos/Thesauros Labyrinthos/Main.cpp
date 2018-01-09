#include "stdafx.h"
#include "mazeGeneration.h"
#include "PlayerCharacter.h"
#include "Camera.h"
using namespace std;

int main() {
	
	

	//! Window creation and mangement
	sf::RenderWindow gameWindow(sf::VideoMode(1600, 900), "2DPhysicsPuzzleGame"); /*!< creates a window and sets it's properties (size) */
	gameWindow.setPosition(sf::Vector2i(0, 9.8));
	gameWindow.setFramerateLimit(60); /*!< set the fps limit for the gamewindow to 60 */

	//below creates a mazegenerator object and then talls it to call the generatemaze function
	MazeGeneration mazeGenerator;
	mazeGenerator.generateMaze();
	//create a playercharacter object and pass the start position of the maze generation since this will always be a floor tile (good place to start).
	PlayerCharacter playerCharacter(mazeGenerator.startX, mazeGenerator.startY);

	//create a a camera object that will be used for the player's view. pass the player positions so that they can be used to set the center of the camera on the player
	Camera playerView(playerCharacter.xPosition, playerCharacter.yPosition); 

	while (gameWindow.isOpen()) {


		gameWindow.clear(sf::Color::Blue);/*!<  clears the window each frame so that it can be updated and old frames aren't left on screen */
					
		
		 //! check all user events
		sf::Event event;
		while (gameWindow.pollEvent(event)) /*!< while the window is checking for events */
		{
			//call the player update function
			playerCharacter.update();
			//update the camera and pass the playerposition so camera can follow.
			playerView.update(playerCharacter.xPosition, playerCharacter.yPosition);

			if (event.type == sf::Event::Closed)/*!< User has pressed the close button(red X) */
			{

				gameWindow.close(); /*!<closes the window */
			}
		}

		//-----------------------------------------------	//Draw section --------------------------------------------------------------------------------------------------------------------------------

		//set the window to use the cemraview
		//gameWindow.setView(playerView.cameraView);
		
		for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
			gameWindow.draw(mazeGenerator.cellsVector[i]->rectangle);
		}
		gameWindow.draw(playerCharacter.rectangle);
		//DISPLAY
		gameWindow.display(); /*!< tells the gamewindow to display everything */
	}


}

