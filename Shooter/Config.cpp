#include "Config.h"

float AsConfig::screen_size_width = 0.0f;
float AsConfig::screen_size_height = 0.0f;

float AsConfig::getScreenWidth ()
{
	if (screen_size_width == 0.0f)
		screen_size_width = static_cast<float>(VideoMode::getDesktopMode().width);

	return screen_size_width;
}

float AsConfig::getScreenHeight ()
{
	if (screen_size_height == 0.0f)
		screen_size_height = static_cast<float>(VideoMode::getDesktopMode().height);

	return screen_size_height;
}
