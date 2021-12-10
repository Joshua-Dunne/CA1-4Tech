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

	Board m_boards[4];
	int m_boardCount = 0;
	bool m_gameFinished = false;
	int m_currentPlayer = 1;

public:
	void run();
	void processInput();
	void update(sf::Time& dt);
	void getInput(sf::Time& dt);
	void checkBoards(sf::Time& dt);
	void render();
};

#endif