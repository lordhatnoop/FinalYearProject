#include "soundManager.h"

SoundManager::SoundManager()
{
	loadSounds(); //load all of the sounds and store them
}

void SoundManager::loadSounds()
{
	//setup the sounds and the music.
	//unlike texture loader, this is more than just laoding the sound, since we can also set whtehr they loop, the volume, etc
	medusaBuffer.loadFromFile("Assets/Sound/medusaAttackCut.wav"); //load the sound into buffer
	medusaAttack.setBuffer(medusaBuffer); //apply it to the sfML sound
	medusaAttack.setVolume(10); //set the volume of the sound 
	

	mainMenuMusic.openFromFile("Assets/Sound/MainTheme.wav");
	mainMenuMusic.setVolume(50);


	skeletonBuffer.loadFromFile("Assets/Sound/Skeleton.wav");
	skeletonSound.setBuffer(skeletonBuffer);
	skeletonSound.setVolume(60); 
}
