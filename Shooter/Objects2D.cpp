#include "Objects2D.h"

Objects2D::Objects2D ()
{
	color_objects = Color(255, 151, 203);

	if (CurrentGame == Game)
	{
		// создание фигур
		// квадраты
		square = VertexArray (createSquare (25, Vector2f (200, 170)));
		square1 = VertexArray (createSquare (30, Vector2f (60, 150)));
		square2 = VertexArray (createSquare (15, Vector2f (125, 125)));
		square3 = VertexArray (createSquare (10, Vector2f (130, 90)));
		// окружности
		circle = VertexArray (createCircle (25, Vector2f (180, 90)));
		// треугольники
		triangle = VertexArray (createTriangle (Vector2f (50, 50), Vector2f (100, 50), Vector2f (100, 100)));
		triangle1 = VertexArray (createTriangle (Vector2f (120, 170), Vector2f (100, 200), Vector2f (160, 205)));
		// стены
		wall_1 = VertexArray (createRectangle (200, 10, Vector2f (130, 20)));
		wall_2 = VertexArray (createRectangle (10, 200, Vector2f (230, 120)));
		wall_3 = VertexArray (createRectangle (200, 10, Vector2f (130, 220)));
		wall_4 = VertexArray (createRectangle (10, 200, Vector2f (30, 120)));
		// окружности по углам стен
		circle1 = VertexArray (createCircle (20, Vector2f (30, 20)));
		circle2 = VertexArray (createCircle (20, Vector2f (230, 20)));
		circle3 = VertexArray (createCircle (20, Vector2f (230, 220)));
		circle4 = VertexArray (createCircle (20, Vector2f (30, 220)));

		// враги
		enemies.push_back(AEnemy(0, Vector2f(150, 50)));
		enemies.push_back(AEnemy(1, Vector2f(200, 150)));
		enemies.push_back(AEnemy(2, Vector2f(150, 170)));
	}
	if (CurrentGame == Test)
	{
		square = VertexArray (createSquare (25, Vector2f (200, 170)));
		line_1 = VertexArray (createLine (Vector2f (500, 500), Vector2f (600, 430)));

		enemies.push_back(AEnemy(0, Vector2f(100, 100)));
		enemies.push_back(AEnemy(1, Vector2f(200, 150)));
		enemies.push_back(AEnemy(2, Vector2f(150, 200)));
	}
}

void Objects2D::Draw (RenderWindow& win)
{
	if (CurrentGame == Game)
	{
		win.draw (square);
		win.draw (square1);
		win.draw (square2);
		win.draw (square3);
		win.draw (triangle);
		win.draw (triangle1);
		win.draw (wall_1);
		win.draw (wall_2);
		win.draw (wall_3);
		win.draw (wall_4);
		win.draw (circle);
		win.draw (circle1);
		win.draw (circle2);
		win.draw (circle3);
		win.draw (circle4);

		// отрисовка активных врагов
		for (auto& enemy : enemies)
		{
			if (enemy.isActive())
			{
				win.draw(enemy.getEnemy());
			}
		}
	}
	if (CurrentGame == Test)
	{
		// отрисовка активных врагов
		for (auto& enemy : enemies)
		{
			if (enemy.isActive())
			{
				win.draw(enemy.getEnemy());
			}
		}

		win.draw (square);
		win.draw (line_1);
	}
}

bool Objects2D::findCrossing (Vector2f v11, Vector2f v12, Vector2f v21, Vector2f v22, Vector2f& result)
{
	Vector2f cut1 = v12 - v11;
	Vector2f cut2 = v22 - v21;

	// Векторное произведение для 2D векторов (просто вычисляем значение)
	float cross1 = cut1.x * (v21 - v11).y - cut1.y * (v21 - v11).x;
	float cross2 = cut1.x * (v22 - v11).y - cut1.y * (v22 - v11).x;
	float cross3 = cut2.x * (v11 - v21).y - cut2.y * (v11 - v21).x;
	float cross4 = cut2.x * (v12 - v21).y - cut2.y * (v12 - v21).x;

	if ((cross1 * cross2 > 0) || (cross1 == 0) || (cross2 == 0))
		return false;

	if ((cross3 * cross4 > 0) || (cross3 == 0) || (cross4 == 0))
		return false;

	// вычисление коэффициента масштабирования "s"
	float s = ((v21.x - v11.x) * (v12.y - v11.y) - (v21.y - v11.y) * (v12.x - v11.x)) / 
				 ((v22.y - v21.y) * (v12.x - v11.x) - (v22.x - v21.x) * (v12.y - v11.y));

	// нахождение точки пересечения
	result.x = s * (v22.x - v21.x) + v21.x;
	result.y = s * (v22.y - v21.y) + v21.y;

	return true;
}

Vector2f Objects2D::checkAllIntersections (Vector2f& start, Vector2f& end, int& texture_type, bool for_collision)
{
	Vector2f local_intersection;
	texture_type = 0; // по умолчанию кирпич

	if (CurrentGame == Game)
	{
		// квадраты
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// квадрат 1
			if (findCrossing (start, end, square[k].position, square[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 1;
			}
			// квадрат 2
			if (findCrossing (start, end, square1[k].position, square1[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 1;
			}
			//квадрат 3
			if (findCrossing (start, end, square2[k].position, square2[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 1;
			}
			// квадрат 4
			if (findCrossing (start, end, square3[k].position, square3[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 1;
			}
		}
		// треугольники
		for (int k = 0, j = 1; k < 3; ++k, ++j)
		{
			// треугольник 1
			if (findCrossing (start, end, triangle[k].position, triangle[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 3;
			}
			// треугольник 2
			if (findCrossing (start, end, triangle1[k].position, triangle1[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 3;
			}
		}
		// стены
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// верхняя стена
			if (findCrossing (start, end, wall_1[k].position, wall_1[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 0;
			}
			// правая стена
			if (findCrossing (start, end, wall_2[k].position, wall_2[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 0;
			}
			// нижняя стена
			if (findCrossing (start, end, wall_3[k].position, wall_3[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 0;
			}
			// левая стена
			if (findCrossing (start, end, wall_4[k].position, wall_4[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 0;
			}
		}
		// окружности
		for (int k = 0, j = 1; k < point_circle + 1; ++k, ++j)
		{
			// окружность 1
			if (findCrossing (start, end, circle[k].position, circle[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 2;
			}
			// окружность у стены левая верхняя
			if (findCrossing (start, end, circle1[k].position, circle1[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 2;
			}
			// окружность у стены правая верхняя
			if (findCrossing (start, end, circle2[k].position, circle2[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 2;
			}
			// окружность у стены правая нижняя
			if (findCrossing (start, end, circle3[k].position, circle3[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 2;
			}
			// окружность у стены левая нижняя
			if (findCrossing (start, end, circle4[k].position, circle4[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 2;
			}
		}
		// враги
		for (auto& enemy : enemies)
		{
			if (!enemy.isActive()) continue;

			Vector2f enemy_points[5];
			enemy.getEnemyPoints(enemy_points);

			for (int k = 0, j = 1; k < 4; ++k, ++j)
			{
				if (findCrossing (start, end, enemy_points[k], enemy_points[j], local_intersection))
				{
					end = local_intersection;
					texture_type = 4;
					enemy_to_delete_id = enemy.getId();
				}
			}
		}
	}
	if (CurrentGame == Test)
	{
		// отрезки
		for (int k = 0, j = 1; k < 2; ++k, ++j)
		{
			// отрезок 1
			if (findCrossing (start, end, line_1[k].position, line_1[j].position, local_intersection))
			{
				end = local_intersection;
			}
		}
		// квадраты
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// квадрат 1
			if (findCrossing (start, end, square[k].position, square[j].position, local_intersection))
			{
				end = local_intersection;
				if (for_collision == false)
					texture_type = 1;
			}
		}
		// враги
		for (auto& enemy : enemies)
		{
			if (!enemy.isActive()) continue;

			Vector2f enemy_points[5];
			enemy.getEnemyPoints(enemy_points);

			for (int k = 0, j = 1; k < 4; ++k, ++j)
			{
				if (findCrossing (start, end, enemy_points[k], enemy_points[j], local_intersection))
				{
					end = local_intersection;
					texture_type = 4;
					enemy_to_delete_id = enemy.getId();
				}
			}
		}
	}

	return end;
}

Vector2f Objects2D::cheskNormales (Vector2f& camera_center, Vector2f &new_position, Vector2f &first_point, Vector2f &second_point)
{
	float collision_distance = 5;
	// дан отрезок AB, точка игрока P, точка предполагаемого смещения P_new
	// A = first_point, B = second_point, P = camera_center, 
	// P_new = (camera_center.x + dx, camera_center.y + dy)

	Vector2f A(first_point.x, first_point.y);
	Vector2f B(second_point.x, second_point.y);
	Vector2f P(camera_center.x, camera_center.y);
	//Vector2f P_new = P + (new_position - P) * delta_time;
	Vector2f P_new = new_position;
	//Vector2f P_new(camera_center.x + dx, camera_center.y + dy);
	Vector2f P_corrected;

	// рассчет расстояния от P до AB
	Vector2f AP = P - A;
	Vector2f AB = B - A;
	
	// проекция AP на AB
	float t = ((AP.x * AB.x) + (AP.y * AB.y)) / ((AB.x * AB.x) + (AB.y * AB.y));
	// ограничение t отрезком [0, 1]
	float t_clamped = std::max(0.0f, std::min(1.0f, t));
	// ближайшаяя точка на отрезке
	Vector2f P_closest = A + t_clamped * AB;
	float distance = sqrt(pow(P.x - P_closest.x, 2) + pow(P.y - P_closest.y, 2));

	// корректировка не требуется
	if (distance > collision_distance)
		return P_corrected = P_new;
	
	// вектор смещения
	Vector2f delta_P = P_new - P;
	// вектор направления
	Vector2f D = B - A;
	// нахождение любой из нормалей (направление поворота на 90градусов ПРОТИВ часовой стрелки)
	Vector2f N_any(-D.y, D.x);
	// скалярное произведение AP и N_any
	float dot = AP.x * N_any.x + AP.y * N_any.y;
	// нормализация нормали
	float N_any_length = sqrt(pow(N_any.x, 2) + pow(N_any.y, 2));
	Vector2f N_unit = N_any / N_any_length;
	// скалярная проекция
	float proj_value = delta_P.x * N_unit.x + delta_P.y * N_unit.y;

	if ((proj_value * dot) < 0)
		P_corrected = P_new - (proj_value * N_unit);
	else
		P_corrected = P_new;

	return P_corrected;
}

Vector2f Objects2D::checkAllCollision (Vector2f& camera_center, float& dx, float& dy, float delta_time)
{
	Vector2f original_new_position = Vector2f(camera_center.x + dx * delta_time, camera_center.y + dy * delta_time);
	Vector2f final_position = original_new_position;

	if (CurrentGame == Game)
	{
		Vector2f best_correction = original_new_position;
		float closest_distance = FLT_MAX;

		// квадраты
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// квадрат 1
			Vector2f correction1 = cheskNormales(camera_center, original_new_position, square[k].position, square[j].position);
			correction1 = handleCornerCase(correction1, original_new_position, square[k].position, square[j].position);
			if (correction1 != original_new_position)
			{
				float correction_strength = abs(correction1.x - original_new_position.x) + 
													 abs(correction1.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction1;
				}
			}
			
			// квадрат 2
			Vector2f correction2 = cheskNormales(camera_center, original_new_position, square1[k].position, square1[j].position);
			correction2 = handleCornerCase(correction2, original_new_position, square1[k].position, square1[j].position);
			if (correction2 != original_new_position)
			{
				float correction_strength = abs(correction2.x - original_new_position.x) + 
													 abs(correction2.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction2;
				}
			}
			
			// квадрат 3
			Vector2f correction3 = cheskNormales(camera_center, original_new_position, square2[k].position, square2[j].position);
			correction3 = handleCornerCase(correction3, original_new_position, square2[k].position, square2[j].position);
			if (correction3 != original_new_position)
			{
				float correction_strength = abs(correction3.x - original_new_position.x) + 
													 abs(correction3.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction3;
				}
			}
			
			// квадрат 4
			Vector2f correction4 = cheskNormales(camera_center, original_new_position, square3[k].position, square3[j].position);
			correction4 = handleCornerCase(correction4, original_new_position, square3[k].position, square3[j].position);
			if (correction4 != original_new_position)
			{
				float correction_strength = abs(correction4.x - original_new_position.x) + 
													 abs(correction4.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction4;
				}
			}
		}
		// треугольники
		for (int k = 0, j = 1; k < 3; ++k, ++j)
		{
			// треугольник 1
			Vector2f correction5 = cheskNormales(camera_center, original_new_position, triangle[k].position, triangle[j].position);
			correction5 = handleCornerCase(correction5, original_new_position, triangle[k].position, triangle[j].position);
			if (correction5 != original_new_position)
			{
				float correction_strength = abs(correction5.x - original_new_position.x) + 
													 abs(correction5.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction5;
				}
			}

			// треугольник 2
			Vector2f correction6 = cheskNormales(camera_center, original_new_position, triangle1[k].position, triangle1[j].position);
			correction6 = handleCornerCase(correction6, original_new_position, triangle1[k].position, triangle1[j].position);
			if (correction6 != original_new_position)
			{
				float correction_strength = abs(correction6.x - original_new_position.x) + 
													 abs(correction6.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction6;
				}
			}
		}
		// стены
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// верхняя стена
			Vector2f correction7 = cheskNormales(camera_center, original_new_position, wall_1[k].position, wall_1[j].position);
			correction7 = handleCornerCase(correction7, original_new_position, wall_1[k].position, wall_1[j].position);
			if (correction7 != original_new_position)
			{
				float correction_strength = abs(correction7.x - original_new_position.x) + 
					abs(correction7.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction7;
				}
			}
			
			// правая стена
			Vector2f correction8 = cheskNormales(camera_center, original_new_position, wall_2[k].position, wall_2[j].position);
			correction8 = handleCornerCase(correction8, original_new_position, wall_2[k].position, wall_2[j].position);
			if (correction8 != original_new_position)
			{
				float correction_strength = abs(correction8.x - original_new_position.x) + 
					abs(correction8.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction8;
				}
			}
			
			// нижняя стена
			Vector2f correction9 = cheskNormales(camera_center, original_new_position, wall_3[k].position, wall_3[j].position);
			correction9 = handleCornerCase(correction9, original_new_position, wall_3[k].position, wall_3[j].position);
			if (correction9 != original_new_position)
			{
				float correction_strength = abs(correction9.x - original_new_position.x) + 
					abs(correction9.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction9;
				}
			}
			
			// левая стена
			Vector2f correction10 = cheskNormales(camera_center, original_new_position, wall_4[k].position, wall_4[j].position);
			correction10 = handleCornerCase(correction10, original_new_position, wall_4[k].position, wall_4[j].position);
			if (correction10 != original_new_position)
			{
				float correction_strength = abs(correction10.x - original_new_position.x) + 
					abs(correction10.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction10;
				}
			}
		}
		// окружности
		for (int k = 0, j = 1; k < point_circle + 1; ++k, ++j)
		{
			// окружность 1
			Vector2f correction11 = cheskNormales(camera_center, original_new_position, circle[k].position, circle[j].position);
			if (correction11 != original_new_position)
			{
				float correction_strength = abs(correction11.x - original_new_position.x) + 
					abs(correction11.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction11;
				}
			}

			// окружность у стены левая верхняя
			Vector2f correction12 = cheskNormales(camera_center, original_new_position, circle1[k].position, circle1[j].position);
			if (correction12 != original_new_position)
			{
				float correction_strength = abs(correction12.x - original_new_position.x) + 
					abs(correction12.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction12;
				}
			}

			// окружность у стены правая верхняя
			Vector2f correction13 = cheskNormales(camera_center, original_new_position, circle2[k].position, circle2[j].position);
			if (correction13 != original_new_position)
			{
				float correction_strength = abs(correction13.x - original_new_position.x) + 
					abs(correction13.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction13;
				}
			}

			// окружность у стены правая нижняя
			Vector2f correction14 = cheskNormales(camera_center, original_new_position, circle3[k].position, circle3[j].position);
			if (correction14 != original_new_position)
			{
				float correction_strength = abs(correction14.x - original_new_position.x) + 
					abs(correction14.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction14;
				}
			}

			// окружность у стены левая нижняя
			Vector2f correction15 = cheskNormales(camera_center, original_new_position, circle4[k].position, circle4[j].position);
			if (correction15 != original_new_position)
			{
				float correction_strength = abs(correction15.x - original_new_position.x) + 
					abs(correction15.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction15;
				}
			}
		}
		// враги
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			for (auto& enemy : enemies)

			{
				if (!enemy.isActive ()) continue;

				Vector2f enemy_points[5];
				enemy.getEnemyPoints (enemy_points);


				Vector2f correction2 = cheskNormales(camera_center, original_new_position, enemy_points[k], enemy_points[j]);
				correction2 = handleCornerCase(correction2, original_new_position, enemy_points[k], enemy_points[j]);

				if (correction2 != original_new_position)
				{
					float correction_strength = abs(correction2.x - original_new_position.x) + 
						abs(correction2.y - original_new_position.y);

					if (correction_strength < closest_distance)
					{
						closest_distance = correction_strength;
						best_correction = correction2;
					}
				}
			}
		}

		final_position = best_correction;
	}
	if (CurrentGame == Test)
	{
		Vector2f best_correction = original_new_position;
		float closest_distance = FLT_MAX;

		// квадраты
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			// квадрат 1
			Vector2f correction1 = cheskNormales(camera_center, original_new_position, square[k].position, square[j].position);
			correction1 = handleCornerCase(correction1, original_new_position, square[k].position, square[j].position);
			
			if (correction1 != original_new_position)
			{
				float correction_strength = abs(correction1.x - original_new_position.x) + 
														abs(correction1.y - original_new_position.y);

				if (correction_strength < closest_distance)
				{
					closest_distance = correction_strength;
					best_correction = correction1;
				}
			}
		}
		// враги
		for (int k = 0, j = 1; k < 4; ++k, ++j)
		{
			for (auto& enemy : enemies)

			{
				if (!enemy.isActive ()) continue;

				Vector2f enemy_points[5];
				enemy.getEnemyPoints (enemy_points);
			

				Vector2f correction2 = cheskNormales(camera_center, original_new_position, enemy_points[k], enemy_points[j]);
				correction2 = handleCornerCase(correction2, original_new_position, enemy_points[k], enemy_points[j]);

				if (correction2 != original_new_position)
				{
					float correction_strength = abs(correction2.x - original_new_position.x) + 
						abs(correction2.y - original_new_position.y);

					if (correction_strength < closest_distance)
					{
						closest_distance = correction_strength;
						best_correction = correction2;
					}
				}
			}
		}

		final_position = best_correction;
	}

	return final_position;
}

Vector2f Objects2D::handleCornerCase (Vector2f correction, Vector2f original_position, Vector2f first_point, Vector2f second_point)
{
	if (correction == original_position)
		return correction;

	const float safe_distance = 7;
	float dist_to_first = sqrt(pow(correction.x - first_point.x, 2) +
										pow(correction.y - first_point.y, 2));
	float dist_to_second = sqrt(pow(correction.x - second_point.x, 2) +
										 pow(correction.y - second_point.y, 2));

	// находится ли исправленная позиция близко к любому углу
	if (dist_to_first < safe_distance || dist_to_second < safe_distance)
	{
		Vector2f closest_angle;
		if (dist_to_first < dist_to_second)
			closest_angle = first_point;
		else
			closest_angle = second_point;

		Vector2f to_correction = correction - closest_angle;
		float to_correction_length = sqrt(to_correction.x * to_correction.x + to_correction.y * to_correction.y);
		if (to_correction_length > 0)
		{
			to_correction.x = to_correction.x / to_correction_length;
			to_correction.y = to_correction.y / to_correction_length;

			correction = closest_angle + to_correction * safe_distance;
		}
	}

	return correction;
}

void Objects2D::isNeedDeleteObject ()
{
	clock_respawn_enemies.restart();
	if (enemy_to_delete_id != -1)
	{
		for (auto& enemy : enemies)
		{
			if (enemy.getId() == enemy_to_delete_id)
			{
				enemy.setActive(false);
				break;
			}
		}
		enemy_to_delete_id = -1;
	}
}

void Objects2D::isNeedCreateObject ()
{
	for (auto& enemy : enemies)
	{
		enemy.setActive(true);
	}
}

VertexArray Objects2D::createSquare (float size, Vector2f center)
{
	float half_square = size / 2;
	VertexArray square(Quads, 5);
	square[0].position = Vector2f(center.x - half_square, center.y - half_square);
	square[1].position = Vector2f(center.x + half_square, center.y - half_square);
	square[2].position = Vector2f(center.x + half_square, center.y + half_square);
	square[3].position = Vector2f(center.x - half_square, center.y + half_square);
	square[4].position = Vector2f(center.x - half_square, center.y - half_square);

	for (int i = 0; i < 5; ++i)
	{
		square[i].color = color_objects;
	}

	return square;
}

VertexArray Objects2D::createCircle (float radius, Vector2f center)
{
	// point_count + 2 так как +1 для центра и +1 для замыкания окружности
	VertexArray circle(TriangleFan, point_circle + 2);
	// центральная точка
	circle[0].position = center;
	circle[0].color = color_objects;

	for (int i = 1; i < point_circle + 2; ++i)
	{
		double angle_circle = (i - 1) * 2 * pi / point_circle;
		float x = center.x + radius * std::cos(angle_circle);
		float y = center.y + radius * std::sin(angle_circle);

		circle[i].position = Vector2f(x, y);
		circle[i].color = color_objects;
	}

	return circle;
}

VertexArray Objects2D::createTriangle (Vector2f first_point, Vector2f second_point, Vector2f third_point)
{
	VertexArray triangle(TriangleFan, 4);

	triangle[0].position = first_point;
	triangle[1].position = second_point;
	triangle[2].position = third_point;
	triangle[3].position = first_point;

	for (int i = 0; i < 4; ++i)
	{
		triangle[i].color = color_objects;
	}

	return triangle;
}

VertexArray Objects2D::createRectangle (float width, float height, Vector2f center)
{
	float half_width = width / 2;
	float half_height = height / 2;

	VertexArray rectangle(Quads, 5);

	rectangle[0].position = Vector2f(center.x - half_width, center.y - half_height);
	rectangle[1].position = Vector2f(center.x + half_width, center.y - half_height);
	rectangle[2].position = Vector2f(center.x + half_width, center.y + half_height);
	rectangle[3].position = Vector2f(center.x - half_width, center.y + half_height);
	rectangle[4].position = Vector2f(center.x - half_width, center.y - half_height);


	for (int i = 0; i < 5; ++i)
	{
		rectangle[i].color = color_objects;
	}

	return rectangle;
}

VertexArray Objects2D::createLine (Vector2f first_point, Vector2f second_point)
{
	VertexArray line(LineStrip, 3);
	
	line[0].position = Vector2f(first_point.x, first_point.y);
	line[1].position = Vector2f(second_point.x, second_point.y);
	line[2].position = Vector2f(first_point.x, first_point.y);

	for (int i = 0; i < 2; ++i)
	{
		line[i].color = color_objects;
	}

	return line;
}
