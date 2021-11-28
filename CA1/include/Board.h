#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <time.h>
#include <stdlib.h>

class Board
{
public:
	Board();

	void update();
	void render();
	void input();

	void endCheck();
	void reset();
	bool m_gameFinished = false;
private:
	int m_boardData[4][4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	int m_row{ 0 };
	int	m_column{ 0 };
	int m_boardCounter{ 0 };

	int m_currentPlayer = 1;


};
#endif