#ifndef BOARD_H
#define BOARD_H

#include <iostream>

class Board
{
public:
	Board();
	
	void update();
	void render();
private:
	int m_boardData[4][4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	bool m_updated{};
};


#endif