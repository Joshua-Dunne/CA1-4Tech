#ifndef BOARD_H
#define BOARD_H

#include <iostream>

class Board
{
public:
	Board();
	
	void update();
	void render();
	void input();
private:
	int m_boardData[4][4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	int m_row{ 0 };
	int	m_column{0};

	int m_currentPlayer = 0;
};
#endif