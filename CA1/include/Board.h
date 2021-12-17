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
	bool input(int t_player, int t_row, int t_col);

#ifndef DEBUG
	void boardFill();
#endif

	void endCheck(int t_currPlayer);
	void reset();

	bool m_boardFinished = false;
	bool m_boardWin = false;
	int m_boardCounter = 0;
	int m_currentPlayer = 1;

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
};
#endif