#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

using namespace sf;

enum class WeaponState
{
	Ready,
	Shooting,
	Reloading,
};


class AsWeapon
{
public:

	AsWeapon();
	void move();
	void draw(RenderWindow &win);
	// установка позиции оружия на экране
	void setGunPosition(int x, int y);
	// выстрел
	void shot();

	// время для анимации выстрела
	Clock clock_anim_shot;
	// переменная для накопления времени
	float elapsed_time = 0;

	WeaponState state;

private:

	struct FrameAnimGun
	{
		int frame_x_pos = 0;
		int frame_y_pos = 0;
		// ширина одного кадра
		int step_x = 457; 
		int step_y = 0;
		int current_frame = 0;
		int total_frame = 9;
	};
	FrameAnimGun Shot_Anim;
	// текстура оружия
	Texture texture_gun;
	// спрайт оружия
	Sprite sprite_gun;

	// анимация выстрела
	void shotAnim();
	// имитация дыхания игрока
	void moveBreath();

	// всего патронов
	int number_bullets;
	// количество патронов в обойме
	int bullets_in_clip;
	// изменение координаты (дыхание)
	int dx = 0, dy = 0;
	// текущие координаты текстуры оружия
	float current_x, current_y;
	// скорость изменения дыхания
	float move_speed = 0.075;
	// амплитуды движения при дыхании
	float move_amplitude_x = 7;
	float move_amplitude_y = 15;
	// напарвление движения оружия при дыхании
	bool to_target = true;
	// угол
	float angle = 0;

	// текстура прицела
	Texture texture_aim;
	// спрайт прицела
	Sprite sprite_aim;
	// время для периодического движения оружия игрока (имитация дыхания)
	Clock clock_gun;
};

