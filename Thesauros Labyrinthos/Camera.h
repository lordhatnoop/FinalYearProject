#pragma once
#include "stdafx.h"
class Camera {
public:
	Camera(int x, int y);
	sf::View cameraView;
	void initCameraView();
	void update(int x, int y);
private:
	int xPosition;
	int yPosition;
};