#include "Board.h"

Board::Board()
{
}

void Board::update()
{
}

void Board::render()
{
    system("cls");
    std::cout << "    B0 B1 B2 B3" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        std::cout << "A" << i << "  ";
        for (int j = 0; j < 4; j++)
        {
            std::cout << m_boardData[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // display player (to cmd)
    switch (m_currentPlayer)
    {
    case 0:
        std::cout << "Player 1 Turn";
        break;
    case 1:
        std::cout << "Player 2 Turn";
        break;
    }

    std::cout << std::endl;
}

void Board::input()
{
    while (true)
    {
        render();
        std::cout << "Select row: A";
        std::cin >> m_row;
        std::cout << "Select row: B";
        std::cin >> m_column;

        if (m_row >= 0 && m_row < 4 && m_column >= 0 && m_column < 4)
        {
            m_boardData[m_row][m_column] = 1; // assigns the player's piece into the board
            // swap current player
            switch (m_currentPlayer)
            {
            case 0:
                m_currentPlayer = 1;
                break;
            case 1:
                m_currentPlayer = 0;
                break;
            }

            render();
            break;
        }
        else
        {
            std::cout << "Error input 0-3 only for rows and columns" << std::endl;
            system("pause");
        }

        

    }
}
