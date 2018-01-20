#pragma once
#include "GameCharacters.h"
#include "TorchLight.h"
class PlayerCharacter : public GameCharacters {

public:
	PlayerCharacter(int x, int y);
	void createSFML();
	void createCollisionBox();
	void update();
	TorchLight* torch;

	float maxTorchFuel = 100.f; // will be upgradable, holds max fuel
	float currentTorchFuel = 100.f;//set this to equal maxTorchfuel at the start to ensure max fuel at the beggining of the game
private:
	void torchCountdown();

};