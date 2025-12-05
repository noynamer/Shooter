#include "Weapon.h"

AsWeapon::AsWeapon ()
	:  current_x(static_cast<int>(AsConfig::getScreenWidth() / 2 - 160)), 
		current_y(static_cast<int>(AsConfig::getScreenHeight() - 745)), state(WeaponState::Ready)
{
	texture_aim.loadFromFile("Image/aim.png");
	sprite_aim.setTexture(texture_aim);
	sprite_aim.setPosition(static_cast<int>(AsConfig::getScreenWidth()) / 2 - 18, static_cast<int>(AsConfig::getScreenHeight()) / 2 - 18);

	texture_gun.loadFromFile("Image/Shotgun/all_frame.png");
	sprite_gun.setTexture(texture_gun);
	sprite_gun.setTextureRect(IntRect(Shot_Anim.frame_x_pos, Shot_Anim.frame_y_pos, Shot_Anim.step_x, 500));
	sprite_gun.scale(1.5, 1.5);
	setGunPosition(current_x, current_y);
}

void AsWeapon::move ()
{
	moveBreath();
}

void AsWeapon::draw (RenderWindow &win)
{
	win.draw(sprite_aim);
	win.draw(sprite_gun);
}

void AsWeapon::setGunPosition (int x, int y)
{
	sprite_gun.setPosition(Vector2f(x, y));
}

void AsWeapon::shot ()
{
	shotAnim();
}

void AsWeapon::shotAnim ()
{
	if (clock_anim_shot.getElapsedTime() > milliseconds(60))
	{
		Shot_Anim.frame_x_pos += Shot_Anim.step_x;
		if (Shot_Anim.frame_x_pos >= 4113)
		{
			Shot_Anim.frame_x_pos = 0;
			state = WeaponState::Ready;
		}
		else
		{
			state = WeaponState::Shooting;
		}

		sprite_gun.setTextureRect (IntRect (Shot_Anim.frame_x_pos, Shot_Anim.frame_y_pos, Shot_Anim.step_x, 500));

		clock_anim_shot.restart ();
	}
}

void AsWeapon::moveBreath ()
{
	if (clock_gun.getElapsedTime() >= milliseconds(16))
	{
		if (to_target)
		{
			angle += move_speed;
			if (angle >= pi) to_target = false;
		}
		else
		{
			angle -= move_speed;
			if (angle <= 0) to_target = true;
		}

		setGunPosition(current_x + move_amplitude_x * cos(angle), current_y + move_amplitude_y * sin(angle));
		clock_gun.restart();
	}
}
