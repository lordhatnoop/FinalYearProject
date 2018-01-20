#include "stdafx.h"
#include "mazeGeneration.h"
#include "PlayerCharacter.h"
#include "Camera.h"
#include "TextureLoader.h"
using namespace std;



TextureLoader textureLoader; //make global  textureLoader object so it can be used elsewhere becasue of the extern
//declartion in the .h file
sf::Shader shader; // create the shader
 
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

	

	//so long as shaders are available to use
	if (sf::Shader::isAvailable) {
		
		shader.loadFromFile("Assets/Shader/Shader.txt", sf::Shader::Fragment); // load the shader file as a fragment

	}

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
		
			//set the uniforms for the shader
			shader.setUniform("current", sf::Shader::CurrentTexture);

			if (event.type == sf::Event::Closed)/*!< User has pressed the close button(red X) */
			{

				gameWindow.close(); /*!<closes the window */
			}
		}

		//-----------------------------------------------	//Draw section --------------------------------------------------------------------------------------------------------------------------------
		

		//create a rectangle that covers the entire screen and is black but somewhat see through so you can sort of see behind it.
		sf::RectangleShape darknessRectangle;
		darknessRectangle.setSize(sf::Vector2f(1600, 900));
		darknessRectangle.setFillColor(sf::Color(0, 0, 0, 240));//black but not opaque
		sf::RenderStates renderState;
		renderState.blendMode = sf::BlendMultiply;

		sf::CircleShape lightCircle;
		lightCircle.setRadius(40);
		lightCircle.setPosition(800, 450);
		lightCircle.setFillColor(sf::Color(255, 255, 255, 100)); // white
		
		
		//set the window to use the cemraview
		//gameWindow.setView(playerView.cameraView);
		
		for (int i = 0; i < mazeGenerator.cellsVector.size(); i++) {
			gameWindow.draw(mazeGenerator.cellsVector[i]->rectangle);
		}
		
		//draw the playersprite and add the shader to it
		gameWindow.draw(playerCharacter.rectangle);
		gameWindow.draw(playerCharacter.torch->torchSprite);
		//gameWindow.draw(textureLoader.darknessSprite);
		//gameWindow.draw(darknessRectangle);
		//gameWindow.draw(lightCircle,renderState);

		//DISPLAY
		gameWindow.display(); /*!< tells the gamewindow to display everything */
	}


}


