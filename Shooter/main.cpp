#include "GameEngine.h"

using namespace sf;

int main()
{
	std::unique_ptr <AsGameEngine> myGame = std::make_unique<AsGameEngine>();
	
	myGame->run();

	return 0;
}
