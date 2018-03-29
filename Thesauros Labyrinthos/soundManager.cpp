#include "soundManager.h"

SoundManager::SoundManager()
{
	loadSounds(); //load all of the sounds and store them
	soundVolume = 100.f;
}

void SoundManager::loadSounds()
{
	//setup the sounds and the music.
	//unlike texture loader, this is more than just laoding the sound, since we can also set whtehr they loop, the volume, etc

	//enemy sounds
	medusaBuffer.loadFromFile("Assets/Sound/medusaAttackCut.wav"); //load the sound into buffer
	medusaAttack.setBuffer(medusaBuffer); //apply it to the sfML sound
	medusaAttack.setVolume((soundVolume/100)*10); //set the volume of the sound . sound volume (current) / by max to get the percentage the volume is set to then times 10 by that to change volume level
	
	skeletonBuffer.loadFromFile("Assets/Sound/Skeleton.wav");
	skeletonSound.setBuffer(skeletonBuffer);
	skeletonSound.setVolume((soundVolume / 100) * 60);


	//menu sounds
	mainMenuMusic.openFromFile("Assets/Sound/MainTheme.wav");
	mainMenuMusic.setVolume((soundVolume / 100) * 50);
	//mainMenuMusic.setVolume(0);

	menuClickBuffer.loadFromFile("Assets/Sound/GUISoundEffectsbyLokif/click_2.wav");
	menuClick.setBuffer(menuClickBuffer);
	menuClick.setVolume((soundVolume / 100) * 50);
	
	//chest sounds
	chestOpenBuffer.loadFromFile("Assets/Sound/MetalLatch.wav");
	chestOpen.setBuffer(chestOpenBuffer);
	chestOpen.setVolume((soundVolume / 100) * 50);
}

void SoundManager::updateSoundVolume()
{
	if (muted == true) {
		//mute all sound
		medusaAttack.setVolume(0); 
		mainMenuMusic.setVolume(0);
		skeletonSound.setVolume(0);
		menuClick.setVolume(0);
		chestOpen.setVolume(0);
	}
	else {
		//update the volumes when needed
		medusaAttack.setVolume((soundVolume / 100) * 10); //set the volume of the sound . sound volume (current) / by max to get the percentage the volume is set to then times 10 by that to change volume level
		mainMenuMusic.setVolume((soundVolume / 100) * 50);
		skeletonSound.setVolume((soundVolume / 100) * 60);
		menuClick.setVolume((soundVolume / 100) * 50);
		chestOpen.setVolume((soundVolume / 100) * 50);
	}
}
