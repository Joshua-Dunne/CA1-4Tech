#include "Board.h"

Board::Board()
{
    m_updated = false;
}

void Board::update()
{
    m_updated = true;
}

void Board::render()
{
    if (m_updated)
    {
        std::cout << "   b0  b1 b2 b3" << std::endl;
        for (int i = 0; i < 4; i++)
        {
            std::cout << "a" << i << "  ";
            for (int j = 0; j < 4; j++)
            {
                std::cout << m_boardData[i][j] << "  ";
            }
            std::cout << std::endl;
        }
        m_updated = false;
    }
}
