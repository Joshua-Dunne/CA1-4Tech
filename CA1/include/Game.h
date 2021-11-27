#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"


class Game
{
public:
	Game();
	~Game();
private:
	sf::RenderWindow m_window;
	sf::Clock m_updateClock;

	Board m_board;

public:
	void run();
	void processInput();
	void update(sf::Time& dt);
	void render();
};

#endif