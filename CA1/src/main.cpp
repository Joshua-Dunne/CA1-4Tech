#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <iostream>
#include "../include/Game.h"

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::string& e)
	{
		std::cerr << e << std::endl;
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "An uncaught error occured!" << std::endl;
	}

	return 0;
}