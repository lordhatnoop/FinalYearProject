#include "Camera.h"

Camera::Camera(int x, int y) {
	//set the position to be the values passed, then init the camera.
	xPosition = x;
	yPosition = y;
	initCameraView();
}

void Camera::initCameraView()
{
	//set the camera view size and center
	cameraView.setCenter(sf::Vector2f(xPosition, yPosition));
	cameraView.setSize(sf::Vector2f(100, 100));
}

void Camera::update(int x, int y)
{
	cameraView.setCenter(sf::Vector2f(x, y)); //keep updating the center of the view to be the value passed(the player position) so that it follows.
}
