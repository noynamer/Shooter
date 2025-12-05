#include "GameEngine.h"

AsGameEngine::AsGameEngine ()
{
	// параметры игрового окна
	m_window.create(VideoMode(m_resolution.x, m_resolution.y), "Shooter", Style::Fullscreen);
	m_state = State::game;
	m_window.setFramerateLimit(60);
}

void AsGameEngine::run()
{
	// объ€вление переменной часы
	sf::Clock clock;
	// цикл работает пока окно открыто
	while (m_window.isOpen())
	{
		// текущее врем€ присваиваетс€ переменной dt
		Time delta_time = clock.restart();
		float dt = delta_time.asSeconds();

		input();
		update(dt);
		draw();
	}
}

void AsGameEngine::input()
{
	// событи€
	Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) m_window.close ();
		
		if (m_state == State::game)
		{
			m_window.setMouseCursorVisible(false);

			if (event.type == Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case Keyboard::W: direction_move = 0; break;
					case Keyboard::S: direction_move = 1; break;
					case Keyboard::A: direction_move = 2; break;
					case Keyboard::D: direction_move = 3; break;
				}
			}
			if (event.type == Event::KeyReleased)
			{
				switch (event.key.code)
				{
					case Keyboard::Escape: m_window.close ();
					case Keyboard::W: direction_move = -1; break;
					case Keyboard::S: direction_move = -1; break;
					case Keyboard::A: direction_move = -1; break;
					case Keyboard::D: direction_move = -1; break;
				}
			}
			if (event.type == Event::MouseButtonPressed)
			{
				switch (event.mouseButton.button)
				{
					case Mouse::Left:
					{
						if (Weapon.state == WeaponState::Ready)
						{
							Weapon.state = WeaponState::Shooting;
							Weapon.clock_anim_shot.restart ();
							Music.playShotgunShot();
							Music.playShotgunRecharge();
							
							if (camera.getDistanceToObjectShot() < 200)
							{
								camera.deleteObjects();
								//camera.createObjects();
							}
						}

						break;
					}
				}
			}
		}

		if (m_state == State::menu)
		{
			if (event.type == Event::MouseButtonPressed)
			{
				Vector2i mouse_pos = Mouse::getPosition(m_window);

				// проверка клика по кнопке "»грать"
				if (Menu.isPlayButtonClicked(mouse_pos))
					m_state = State::game;
				// проверка клика по полю ввода
				if (Menu.isInputBoxClicked(mouse_pos))
				{
					Menu.setInputActive(true);
					Menu.setInputBoxOutlineColor(Color::Red);
				}
				else
				{
					Menu.setInputActive(false);
					Menu.setInputBoxOutlineColor(Color::White);
				}
				// проверка клика по кнопке "Enter"
				if (Menu.isEnterButtonClicked(mouse_pos))
				{
					if (!Menu.getInputString().empty())
					{
						try
						{
							int number = std::stoi(Menu.getInputString());
							Menu.setNumberValue(number);
							Menu.setInputBoxOutlineColor(Color::Green);
						}
						catch (...) 
						{
							Menu.setInputBoxOutlineColor(Color::Red);
						}
					}
				}
			}

			if (event.type == Event::TextEntered && Menu.getInputActive())
			{
				std::string current_text = Menu.getInputString();

				// Backspace
				if (event.text.unicode == 8)
				{
					if (!current_text.empty())
						current_text.pop_back();
				}
				// только цифры
				else if (event.text.unicode >= 48 && event.text.unicode <= 57)
				{
					if (current_text.length() < 5)
						current_text += static_cast<char>(event.text.unicode);
				}

				Menu.setInputString(current_text);
				Menu.setInputTextString(current_text);
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Enter && Menu.getInputActive())
				{
					if (!Menu.getInputString().empty())
					{
						try
						{
							int number = std::stoi (Menu.getInputString ());
							Menu.setNumberValue (number);
							Menu.setInputBoxOutlineColor (Color::Green);
						}
						catch (...) 
						{
							Menu.setInputBoxOutlineColor(Color::Red);
						}
					}
				}
			}
		}
	}
}

void AsGameEngine::update(float const &deltaTime)
{
	if (m_state == State::game)
	{
		if (Weapon.clock_anim_shot.getElapsedTime() > milliseconds(60) && Weapon.state == WeaponState::Shooting)
		{
			Weapon.shot ();
		}

		camera.Move (direction_move, deltaTime, Music);
		Weapon.move ();

		camera.createObjects();

	}
}

void AsGameEngine::draw()
{
	if (m_state == State::game)
	{
		m_window.clear();

		camera.wallColumns(m_window);
		camera.mouseRotation(m_window);
		camera.Draw(m_window);
		Weapon.draw(m_window);

		m_window.display();
	}
	if (m_state == State::menu)
	{
		Menu.Draw(m_window);
	}
}

