#include"include\Game.h"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
	Game* game = new Game();
	delete game;
		
	return 0;
}