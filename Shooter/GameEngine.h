#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Weapon.h"
#include "Menu.h"
#include "Music.h"

class AsGameEngine
{
public:

	AsGameEngine();
	// метод запуска игрового цикла
	void run();

private:

	AsConfig Config;

	enum class State
	{
		menu, game
	};
	State m_state;

	// разрешение экрана
	Vector2f m_resolution = Vector2f(static_cast<float>(VideoMode::getDesktopMode().width),
		static_cast<float>(VideoMode::getDesktopMode().height));
	// графическое окно
	sf::RenderWindow m_window;

	AsMenu Menu;
	AsWeapon Weapon;
	Camera camera;
	AsMusic Music;

	int direction_move = -1;

	// метод обработки событий
	void input();
	// метод обновлния переменных, свойств и методов
	void update(float const &deltaTime);
	// метод отрисовки объектов в графическом окне
	void draw();
};

