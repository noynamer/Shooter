#pragma once

#include "Config.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Objects2D
{
public:

	Objects2D();
	void Draw (RenderWindow& win);
	// пересечение лучей с препятствиями
	bool findCrossing(Vector2f v11, Vector2f v12, Vector2f v21, Vector2f v22, Vector2f& result);
	// проверка пересечения лучей со всеми объектами
	Vector2f checkAllIntersections(Vector2f &start, Vector2f &end, int& texture_type, bool for_collision);
	// расчет нормалей к поверхностям объектов
	Vector2f cheskNormales(Vector2f& camera_center, Vector2f &new_position, Vector2f &first_point, Vector2f &second_point);
	// проверка коллизий с объетами и корректировка движения игрока
	Vector2f checkAllCollision(Vector2f& camera_center, float& dx, float& dy, float delta_time);
	// обработка углов отрезков столкновения
	Vector2f handleCornerCase(Vector2f correction, Vector2f original_position, Vector2f first_point, Vector2f second_point);

	void isNeedDeleteObject();
	void isNeedCreateObject();

	// количество точек в окружности
	int point_circle = 20;

	// квадраты
	VertexArray square, square1, square2, square3;
	// окружности
	VertexArray circle, circle1, circle2, circle3, circle4;
	// треуголькики
	VertexArray triangle, triangle1;
	// прямоугольники стен
	VertexArray wall_1, wall_2, wall_3, wall_4;
	
	// тестовые линии
	VertexArray line_1;

	// время для перереспавна врагов
	Clock clock_respawn_enemies;

private:
	// режим программы
	enum TypeGame
	{
		// тестовый режим
		Test,
		// игровой режим
		Game,
	};
	TypeGame CurrentGame = Game;

	// цвет фигур на карте
	Color color_objects;

	// обычные фигуры
	VertexArray createSquare(float size, Vector2f center);
	VertexArray createCircle(float radius, Vector2f center);
	VertexArray createTriangle(Vector2f first_point, Vector2f second_point, Vector2f third_point);
	VertexArray createRectangle(float width, float height, Vector2f center);
	VertexArray createLine(Vector2f first_point, Vector2f second_point);

	vector<AEnemy> enemies;
	int enemy_to_delete_id = -1;
};

