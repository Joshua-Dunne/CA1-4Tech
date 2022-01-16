#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <SFML/Graphics.hpp>

class Board
{
public:
	Board();

	friend class Evaluator;
	friend class IsometricBoard;
	friend class AI;

	void update(sf::Time dt);
	void render(sf::RenderWindow& t_window);

	void endCheck(int t_currPlayer);
	void reset();
	std::vector<std::pair<int, int>> getVaildMoves();

	bool m_boardFinished = false;
	bool m_boardWin = false;
	int m_boardCounter = 0;
	int m_currentPlayer = 1;

	
private:
	int m_boardData[16][4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	}; 

	sf::Text m_winnerText;
	sf::Font m_font;
	float m_timer = 0.0f;
	bool m_winTextCheck{ false };
};
#endif