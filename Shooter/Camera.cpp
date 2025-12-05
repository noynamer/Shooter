#include "Camera.h"
#include <cmath>

Camera::Camera ()
	//: center(550, 450), dx (0), dy (0)
	: center(125, 70), dx (0), dy (0)
{
	screen_width = static_cast<int>(AsConfig::getScreenWidth());
	screen_height = static_cast<int>(AsConfig::getScreenHeight());

	ray.count_ray_view = screen_width;
	textureTypes.resize(ray.count_ray_view);
	ray.angle_ray.resize(static_cast<int>(AsConfig::getScreenWidth()));
	distanceToObject.resize(static_cast<int>(AsConfig::getScreenWidth()));

	ray_shot.count_ray_view = 2;
	ray_shot.angle_ray.resize(ray_shot.count_ray_view);
	distanceToObjectShot.resize(2);

	Redraw();
}

void Camera::Move (int number_move, float delta_time, AsMusic &Music)
{
	// на месте
	if (number_move == -1)
	{
		dx = 0;
		dy = 0;
		is_moving = false;
		step_timer = -0.1;
	}
	else
	{
		is_moving = true;
		step_timer += delta_time;
		if (step_timer >= step_interval || step_timer < 0)
		{
			Music.playStep();
			step_timer = 0;
		}
	}
	// вперед
	if (number_move == 0)
	{
		dx = move_speed * std::cos(rotation);
		dy = move_speed * std::sin(rotation);
	}
	// назад
	if (number_move == 1)
	{
		dx = -move_speed * std::cos(rotation);
		dy = -move_speed * std::sin(rotation);
	}
	// влево
	if (number_move == 2)
	{
		dx = -move_speed * std::sin(-rotation);
		dy = -move_speed * std::cos(-rotation);
	}
	// вправо
	if (number_move == 3)
	{
		dx = move_speed * std::sin(-rotation);
		dy = move_speed * std::cos(-rotation);
	}
	// вверх вправо
	if (number_move == 4)
	{
		dx = move_speed * std::sin(-rotation);
		dy = -move_speed * std::cos(-rotation);
	}

	Vector2f new_position = Objects.checkAllCollision(center, dx, dy, delta_time);
	center = new_position;
	
	Redraw ();
}

void Camera::Draw (RenderWindow& win)
{
	win.draw(ray.lines);

	win.draw(ray_shot.lines);

	win.draw(circle_camera);
	Objects.Draw(win);
}

void Camera::Ray ()
{
	// создание лучей игрока
	ray.lines = VertexArray(Lines, ray.count_ray_view);
	ray_shot.lines = VertexArray(Lines, ray_shot.count_ray_view);

	// ветор результатов
	vector<RayResult> results(ray.count_ray_view);
	vector<RayResult> results_shot(ray_shot.count_ray_view);

	const int mid = ray.count_ray_view / 2;

	thread th1(&Camera::rayWorker, this, 0, 0.25 * mid, std::ref(results), false);
	thread th2(&Camera::rayWorker, this, 0.25 * mid, 0.55 * mid, std::ref(results), false);
	thread th3(&Camera::rayWorker, this, 0.5 * mid, 0.75 * mid, std::ref(results), false);
	thread th4(&Camera::rayWorker, this, 0.75 * mid, mid, std::ref(results), false);
	thread th5(&Camera::rayWorker, this, mid, 1.25 * mid, std::ref(results), false);
	thread th6(&Camera::rayWorker, this, 1.25 * mid, 1.5 * mid, std::ref(results), false);
	thread th7(&Camera::rayWorker, this, 1.5 * mid, 1.75 * mid, std::ref(results), false);
	thread th8(&Camera::rayWorker, this, 1.75 * mid, 2 * mid, std::ref(results), false);
	thread th9(&Camera::rayWorker, this, 0, 1, std::ref(results_shot), true);

	th1.join();
	th2.join();
	th3.join();
	th4.join();
	th5.join();
	th6.join();
	th7.join();
	th8.join();
	th9.join();

	for (int i = 0; i < ray.count_ray_view; i += 2)
	{
		ray.lines[i].position = results[i].start;
		ray.lines[i + 1].position = results[i].end;
		ray.lines[i].color = results[i].color;
		ray.lines[i + 1].color = results[i].color;
		distanceToObject[i] = results[i].distance;
		distanceToObject[i + 1] = results[i].distance;

		textureTypes[i] = results[i].texture_type;
		textureTypes[i + 1] = results[i].texture_type;
	}

	for (int i = 0; i < ray_shot.count_ray_view; i += 2)
	{
		ray_shot.lines[i].position = results_shot[i].start;
		ray_shot.lines[i + 1].position = results_shot[i].end;
		ray_shot.lines[i].color = results_shot[i].color;
		ray_shot.lines[i + 1].color = results_shot[i].color;
		distanceToObjectShot[i] = results_shot[i].distance;
		distanceToObjectShot[i + 1] = results_shot[i].distance;

		textureTypes[i] = results_shot[i].texture_type;
		textureTypes[i + 1] = results_shot[i].texture_type;
	}
}

void Camera::cameraCircle ()
{
	// окружность
	circle_camera.setPosition(Vector2f(center.x, center.y));
	circle_camera.setRadius(radius_camera);
	circle_camera.setOrigin(radius_camera, radius_camera);
	circle_camera.setFillColor(Color::Red);
	circle_camera.setOutlineThickness(1.5);
	circle_camera.setOutlineColor(Color::White);
}

void Camera::wallColumns (RenderWindow& win)
{
	// загрузка текстур
	static Texture brick_texture, steel_texture, stone_texture, tree_texture, sky_texture, floor_texture, enemy_texture;
	static bool loaded = brick_texture.loadFromFile("Image/brick.jpg") &&
								steel_texture.loadFromFile("Image/steel.jpg") && 
								stone_texture.loadFromFile("Image/stone.jpg") && 
								tree_texture.loadFromFile("Image/tree.jpg") &&
								enemy_texture.loadFromFile("Image/player (1).jpg");

	// массивы для каждой текстуры
	VertexArray brick_mass(Quads), steel_mass(Quads), stone_mass(Quads), tree_mass(Quads), enemy_mass(Quads);
	VertexArray floor_mass(Quads, screen_width * 4);
	VertexArray sky_mass(Quads, screen_width * 4);

	// предварительный подсчет количества вершин для каждой текстуры
	int brick_count = 0, steel_count = 0, stone_count = 0, tree_count = 0, enemy_count = 0;
	int floor_count = screen_width * 4, sky_count = screen_width * 4;
	for (int i = 0; i < screen_width; ++i)
	{
		switch (textureTypes[i])
		{
			case 0: brick_count += 4; break;
			case 1: steel_count += 4; break;
			case 2: stone_count += 4; break;
			case 3: tree_count += 4; break;
			case 4: enemy_count += 4; break;
		}
	}

	// изменение размеров массивов
	brick_mass.resize(brick_count);
	steel_mass.resize(steel_count);
	stone_mass.resize(stone_count);
	tree_mass.resize(tree_count);
	floor_mass.resize(floor_count);
	sky_mass.resize(sky_count);
	enemy_mass.resize(enemy_count);

	// индексы для массивов
	int brick_indedx = 0, steel_indedx = 0, stone_indedx = 0, tree_indedx = 0, enemy_indedx = 0;
	
	// обработка неба
	for (int i = 0; i < screen_width; ++i)
	{
		float horizon_line = screen_height / 2;
		float sky_top = 0;

		float center_dist = abs(i - screen_width / 2) / (screen_width / 2);
		float sky_bottom = horizon_line - horizon_line * center_dist;


		float texture_x = 0;
		float texture_y_bottom = 1;
		float texture_y_top = 0;

		Color sky_color(100, 150, 255);

		// вершины неба
		int index = i * 4;
		sky_mass[index] = Vertex(Vector2f(i, sky_top), sky_color, Vector2f(texture_x, texture_y_top));
		sky_mass[index + 1] = Vertex(Vector2f(i + 1, sky_top), sky_color, Vector2f(texture_x + 1, texture_y_top));
		sky_mass[index + 2] = Vertex(Vector2f(i + 1, sky_bottom), sky_color, Vector2f(texture_x + 1, texture_y_bottom));
		sky_mass[index + 3] = Vertex(Vector2f(i, sky_bottom), sky_color, Vector2f(texture_x, texture_y_bottom));
	}

	// обработка пола
	for (int i = 0; i < screen_width; ++i)
	{
		float horizon_line = screen_height / 2;
		float floor_bottom = screen_height;

		float center_dist = abs(i - screen_width / 2) / (screen_width / 2);
		float floor_top = horizon_line + (floor_bottom - horizon_line) * center_dist;


		float texture_x = 0;
		float texture_y_bottom = 1.0;
		float texture_y_top = 0;

		Color floor_color(136, 136, 136);

		// вершины пола
		int index = i * 4;
		floor_mass[index] = Vertex(Vector2f(i, floor_top), floor_color, Vector2f(texture_x, texture_y_top));
		floor_mass[index + 1] = Vertex(Vector2f(i + 1, floor_top), floor_color, Vector2f(texture_x + 1, texture_y_top));
		floor_mass[index + 2] = Vertex(Vector2f(i + 1, floor_bottom), floor_color, Vector2f(texture_x + 1, texture_y_bottom));
		floor_mass[index + 3] = Vertex(Vector2f(i, floor_bottom), floor_color, Vector2f(texture_x, texture_y_bottom));
	}

	// обработка стен
	for (int i = 0; i < screen_width; ++i)
	{
		float dist = min(distanceToObject[i], radius_view);
		// угол между лучом и направлением игрока
		float angle_diff = ray.angle_ray[i] - rotation;
		// коррекция рыбьего глаза
		float corrected_distanse = dist * cos(angle_diff);

		// высота столбца (коэффициенты 10 и 2.5 подбирались эмпирически)
		float column_height = screen_height * 10 / (corrected_distanse + 2.5);
		// позиция по y (центрирование (равные отступы сверху и снизу от середины экрана))
		float pos_y = (screen_height - column_height) / 2;

		// корректное затемнение
		float darkness = 1 - (dist / radius_view);
		// выбор цвета в зависимости от расстояния
		int color_value = (200 * darkness);
		Color column_color(color_value, color_value, color_value);

		// выбор текстуры и соответствующего массива
		VertexArray* current_array = &brick_mass;
		Texture* current_texture = &brick_texture;
		int* current_index = &brick_indedx;
		float texture_height = brick_texture.getSize().y;

		// стены и враги
		bool is_for_enemy = false;
		switch (textureTypes[i])
		{
			// кирпич
			case 0:
				current_array = &brick_mass;
				current_texture = &brick_texture;
				current_index = &brick_indedx;
				texture_height = brick_texture.getSize().y;
				break;
			// сталь
			case 1:
				current_array = &steel_mass;
				current_texture = &steel_texture;
				current_index = &steel_indedx;
				texture_height = steel_texture.getSize().y;
				break;
			// камень
			case 2:
				current_array = &stone_mass;
				current_texture = &stone_texture;
				current_index = &stone_indedx;
				texture_height = stone_texture.getSize().y;
				break;
			// дерево
			case 3:
				current_array = &tree_mass;
				current_texture = &tree_texture;
				current_index = &tree_indedx;
				texture_height = tree_texture.getSize().y;
				break;
			// враги
			case 4:
				current_array = &enemy_mass;
				current_texture = &enemy_texture;
				current_index = &enemy_indedx;
				texture_height = enemy_texture.getSize().y;
				is_for_enemy = true;
				break;
		}

		// зацикливание текстуры по горизонтали
		float texture_x = calculateTextureX(i);

		// добавление вершин в соответствующие массивы
		int vertex_index = *current_index;

		(*current_array)[vertex_index] = Vertex(Vector2f(i, pos_y), Vector2f(texture_x, 0));
		(*current_array)[vertex_index + 1] = Vertex(Vector2f(i + 1, pos_y), Vector2f(texture_x + 1, 0));
		(*current_array)[vertex_index + 2] = Vertex(Vector2f(i + 1, pos_y + column_height), Vector2f(texture_x + 1, texture_height));
		(*current_array)[vertex_index + 3] = Vertex(Vector2f(i, pos_y + column_height), Vector2f(texture_x, texture_height));
		
		for (int j = 0; j < 4; ++j)
		{
			(*current_array)[vertex_index + j].color = column_color;
		}

		*current_index +=4;
	}

	// отрисовка
	RenderStates states;

	// небо
	if (sky_mass.getVertexCount() > 0)
	{
		win.draw(sky_mass);
	}
	// пол
	if (floor_mass.getVertexCount() > 0)
	{
		win.draw(floor_mass);
	}
	// стены
	if (brick_mass.getVertexCount() > 0)
	{
		states.texture = &brick_texture;
		win.draw(brick_mass, states);
	}
	if (steel_mass.getVertexCount() > 0)
	{
		states.texture = &steel_texture;
		win.draw(steel_mass, states);
	}
	if (stone_mass.getVertexCount() > 0)
	{
		states.texture = &stone_texture;
		win.draw(stone_mass, states);
	}
	if (tree_mass.getVertexCount() > 0)
	{
		states.texture = &tree_texture;
		win.draw(tree_mass, states);
	}
	// враги
	if (enemy_mass.getVertexCount() > 0)
	{
		states.texture = &enemy_texture;
		win.draw(enemy_mass, states);
	}
}

void Camera::mouseRotation (RenderWindow& win)
{
	Vector2i mouse_pos = Mouse::getPosition(win);

	int center_x = win.getSize().x / 2;
	int center_y = win.getSize().y / 2;
	int delta_x = mouse_pos.x - center_x;

	rotation += delta_x * mouse_sensitivity;
	if (rotation >= 2 * pi)
	{
		rotation = 0;
	}
	if (rotation <= -2 * pi)
	{
		rotation = 0;
	}

	Mouse::setPosition(Vector2i(center_x, center_y), win);

	Redraw ();
}

float Camera::getDistanceToObjectShot ()
{
	return distanceToObjectShot[1];
}

void Camera::deleteObjects ()
{
	int center_index = ray.count_ray_view / 2;

	if (textureTypes[center_index] == 4)
	{
		Objects.isNeedDeleteObject ();
		is_need_create_enemies = true;
	}
}

void Camera::createObjects ()
{
	if (Objects.clock_respawn_enemies.getElapsedTime() > seconds(3))
	{
		if (is_need_create_enemies == true)
		{
			Objects.isNeedCreateObject ();
		}
	}
}

void Camera::rayWorker (int from, int to, vector<RayResult>& results, bool for_collision)
{
	for (int i = from; i < to; i ++)
	{
		// переменная типа текстуры
		int texture_type = 0;

		if (for_collision == false)
		{
			ray.angle_ray[i] = rotation - field_of_view_angle / 2 + i * field_of_view_angle / ray.count_ray_view;
			Vector2f ray_end = Vector2f (center.x + radius_view * std::cos (ray.angle_ray[i]),
												  center.y + radius_view * std::sin (ray.angle_ray[i]));

			// проверка пересечений со всеми объектами
			Vector2f final_point = Objects.checkAllIntersections (center, ray_end, texture_type, false);

			results[i] = {
				Vector2f (center.x, center.y),
				final_point,
				Color (255, 185, 220, 50),
				sqrt (((center.x - final_point.x) * (center.x - final_point.x)) +
						((center.y - final_point.y) * (center.y - final_point.y))),
				texture_type
			};
		}

		if (for_collision == true)
		{
			ray_shot.angle_ray[i] = rotation;
			Vector2f ray_end = Vector2f (center.x + radius_view * std::cos (ray_shot.angle_ray[i]),
												  center.y + radius_view * std::sin (ray_shot.angle_ray[i]));

			// проверка пересечений со всеми объектами
			Vector2f final_point = Objects.checkAllIntersections (center, ray_end, texture_type, true);

			results[i] = {
				Vector2f (center.x, center.y),
				final_point,
				Color (0, 255, 0),
				sqrt (((center.x - final_point.x) * (center.x - final_point.x)) +
						((center.y - final_point.y) * (center.y - final_point.y))),
				texture_type
			};
		}
	}
}

void Camera::Redraw ()
{
	// камера
	cameraCircle();
	// лучи
	Ray();
}

float Camera::calculateTextureX (int screen_x)
{
	int texture_width = 0;
	float distance = distanceToObject[screen_x];

	if (textureTypes[screen_x] == 0) texture_width = 300;
	if (textureTypes[screen_x] == 1) texture_width = 75;
	if (textureTypes[screen_x] == 2) texture_width = 565;
	if (textureTypes[screen_x] == 3) texture_width = 300;
	if (textureTypes[screen_x] == 4) texture_width = 300;

	// если есть пересечение
	if (distance < radius_view)
	{
		float ray_angle = ray.angle_ray[screen_x];

		// мировые координаты точки пересечения
		Vector2f intersection = center + Vector2f(cos(ray_angle), sin(ray_angle)) * distance;

		float texture_coord = fmod(intersection.x + intersection.y, 1);
		return texture_coord * texture_width;
	}
	
	// если нет пересечения
	return screen_width % texture_width;
}
