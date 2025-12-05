#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Config.h"

using namespace sf;

class AsMusic
{
public:

	AsMusic();
	void playStep();
	void playShotgunShot();
	void playShotgunRecharge();

private:
	// звук шагов игрока
	Sound player_step, shotgun_shot, shotgun_recharge;
	SoundBuffer step, shot, recharge;
};

