#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class AEnemy
{
public:

	AEnemy() = delete;
	AEnemy(int enemy_id, Vector2f pos);
	// создание квадрата врага
	VertexArray createEnemy (float size, Vector2f center);
	void Draw(RenderWindow& win);
	VertexArray getEnemy();
	int getId() const;
	void setActive(bool active);
	bool isActive() const;
	void getEnemyPoints(Vector2f points[5]) const;

private:

	int id;
	bool is_active = true;
	VertexArray enemy_shape;

	// цвет
	Color color_enemy;
	// координаты 
	Vector2f position;
	// скорость 
	//float speed;
	// здоровье 
	//float hp;
};

