#pragma once

#include "Config.h"
#include "Objects2D.h"
#include "Music.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>

using namespace sf;

class Camera
{
public:

	Camera();
	void Move(int number_move, float delta_tim, AsMusic &Music);
	void Draw (RenderWindow& win);
	// основная логика лучей
	void Ray();
	// логика камеры
	void cameraCircle();
	// логика столбцов (имитация 3D)
	void wallColumns(RenderWindow& win);
	// поворот экрана мышью
	void mouseRotation(RenderWindow& win);
	float getDistanceToObjectShot();
	// удаление врагов при попадании
	void deleteObjects();
	// создание врагов
	void createObjects();

private:

	int screen_width;
	int screen_height;

	float step_timer = -0.1;
	// интервал между звуком шагов в секундах
	float step_interval = 0.45;
	bool is_moving = false;

	struct Rays
	{
		VertexArray lines;
		// угол конкретной точки (в радианах), используемый для обзора и отрисовки на экране объектов
		vector<float> angle_ray;
		// количество лучей, используемых для обзора и отрисовки на экране объектов
		int count_ray_view;
	};
	struct RayResult
	{
		Vector2f start;
		Vector2f end;
		Color color;
		float distance;
		// тип текстуры объекта (0 = кирпич, 1 = сталь, 2 = камень, 3 = дерево, 4 = враг)
		int texture_type;
	};

	// геометрические фигуры на карте 
	Objects2D Objects;
	// лучи игрока, используемые для обзора и отрисовки на экране объектов
	Rays ray;
	// лучи игрока, используемые для обработки выстрела
	Rays ray_shot;

	// логика потока
	void rayWorker(int from, int to, vector<RayResult> &results, bool for_collision);
	// перерисовка
	void Redraw();
	// корректное вычисление координаты текстуры
	float calculateTextureX(int screen_x);

	// окгужность камеры
	CircleShape circle_camera;
	// центр камеры
	Vector2f center;
	// радиус круга камеры
	float radius_camera = 5;
	// расстояние от центра камеры до объекта
	float distance_to_object;
	// радиус видимости, используемый для обзора и отрисовки на экране объектов
	const float radius_view = 250;
	// угол обзора, используемый для обзора и отрисовки на экране объектов
	double field_of_view_angle = pi / 2;
	// направление взгляда камеры (куда смотрит центр камеры)
	float rotation;

	// скорость движения камеры
	//float move_speed = 1.5f;
	float move_speed = 35.f;
	// скорость поворота камеры
	float rotate_speed = 0.05f;
	// чуствительность мыши
	float mouse_sensitivity = 0.002;
	// смещение движения камеры
	float dx, dy;
	// вектор расстояний до объекта, используемых для обзора и отрисовки на экране объектов
	vector<float> distanceToObject;
	// вектор расстояний до объекта, используемых для выстрелов
	vector<float> distanceToObjectShot;

	// массив для хранения типов текстур для каждого луча
	vector<int> textureTypes;

	// нужно ли создавать заново объекты врагов
	bool is_need_create_enemies = false;
};

