#include "Music.h"

AsMusic::AsMusic ()
{
	// шаги игрока
	step.loadFromFile("Music/player_step_cut.wav");
	player_step.setBuffer(step);
	// выстрел из дробовика
	shot.loadFromFile("Music/Shotgun/good-close-single-shot (mp3cut.net) (1).wav");
	shotgun_shot.setBuffer(shot);
	// перезарядка дробовика
	recharge.loadFromFile("Music/Shotgun/shotgun-being-reloaded (mp3cut.net) (2).wav");
	shotgun_recharge.setBuffer(recharge);
}

void AsMusic::playStep ()
{
	player_step.play();
}

void AsMusic::playShotgunShot ()
{
	shotgun_shot.play();
}

void AsMusic::playShotgunRecharge ()
{
	shotgun_recharge.play();
}
