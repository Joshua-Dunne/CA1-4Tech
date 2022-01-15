#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Evaluator.h"
#include "IsometricBoard.h"
#include "AI.h"


class Game
{
public:
	Game();
	~Game();
private:
	sf::RenderWindow m_window;
	sf::Clock m_updateClock;

	Board m_board;
	Evaluator eval;
	IsometricBoard m_isoBoard;
	bool m_gameFinished = false;
	bool m_gameWon = false;
	bool m_playMade = false;
	int m_currentPlayer = 1;
	std::vector<std::pair<int, int>> m_vaildMoves; // storing the moves
	AI* aiPlayer;

public:
	void run();
	void processInput();
	void update(sf::Time& dt);
	void checkBoards(sf::Time& dt);
	void render();
	void resetGame();

	bool AI_VS_AI = false;
};

#endif