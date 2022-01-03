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
	bool m_gameFinished = false;
	bool m_gameWon = false;
	int m_currentPlayer = 1;
	std::vector<std::pair<int, int>> m_vaildMoves; // storing the moves

public:
	void run();
	void processInput();
	void update(sf::Time& dt);
	void getInput(sf::Time& dt);
	void checkBoards(sf::Time& dt);
	void render();
	void resetGame();
};

#endif