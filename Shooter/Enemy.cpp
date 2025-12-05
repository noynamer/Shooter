#include "Enemy.h"

AEnemy::AEnemy (int enemy_id, Vector2f pos)
	:	id(enemy_id), position(pos)
{
	color_enemy = Color(237, 28, 36);

	enemy_shape = createEnemy(5, position);
}

VertexArray AEnemy::createEnemy (float size, Vector2f center)
{
	float half_square_enemy = size / 2;
	VertexArray enemy(Quads, 5);
	enemy[0].position = Vector2f(center.x - half_square_enemy, center.y - half_square_enemy);
	enemy[1].position = Vector2f(center.x + half_square_enemy, center.y - half_square_enemy);
	enemy[2].position = Vector2f(center.x + half_square_enemy, center.y + half_square_enemy);
	enemy[3].position = Vector2f(center.x - half_square_enemy, center.y + half_square_enemy);
	enemy[4].position = Vector2f(center.x - half_square_enemy, center.y - half_square_enemy);

	for (int i = 0; i < 5; ++i)
	{
		enemy[i].color = color_enemy;
	}

	return enemy;
}

void AEnemy::Draw (RenderWindow& win)
{
	win.draw (enemy_shape);
}

VertexArray AEnemy::getEnemy ()
{
	return enemy_shape;
}

int AEnemy::getId () const
{
	return id;
}

void AEnemy::setActive (bool active)
{
	is_active = active;
}

bool AEnemy::isActive () const
{
	return is_active;
}

void AEnemy::getEnemyPoints (Vector2f points[5]) const
{
	points[0] = enemy_shape[0].position;
	points[1] = enemy_shape[1].position;
	points[2] = enemy_shape[2].position;
	points[3] = enemy_shape[3].position;
	points[4] = enemy_shape[4].position;
}
