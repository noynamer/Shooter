#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

// число pi
const double pi = 4.0 * std::atan(1.0);

class AsConfig
{
public:

	static float getScreenWidth();
	static float getScreenHeight();

private:
	// ширина экрана
	static float screen_size_width;
	// высота экрана
	static float screen_size_height;
};
