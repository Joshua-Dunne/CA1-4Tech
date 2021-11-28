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
    case 1:
        std::cout << "Player 1 Turn";
        break;
    case 2:
        std::cout << "Player 2 Turn";
        break;
    default:
        std::cout << "error displaying player turn";
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
            if (m_boardData[m_row][m_column] != 1 && m_boardData[m_row][m_column] != 2)
            { // check to make sure that the current info trying to be placed doesn't exist already
                // swap current player
                switch (m_currentPlayer)
                {
                case 1:
                    m_boardData[m_row][m_column] = 1; // assigns the player's piece into the board
                    m_currentPlayer = 2;
                    break;
                case 2:
                    m_boardData[m_row][m_column] = 2; // assigns the player's piece into the board
                    m_currentPlayer = 1;
                    break;
                default:
                    std::cout << "error choosing player piece to place, defaulting to player 1 placing piece" << std::endl;
                    m_boardData[m_row][m_column] = 1; // assigns the player's piece into the board
                    m_currentPlayer = 2;
                }

                render();
                break;
            }
            else
            {
                std::cout << "Error! A" << m_row << ", B" << m_column << " already occupied!" << std::endl;
                system("pause");
            }
            
            
        }
        else
        {
            std::cout << "Error input 0-3 only for rows and columns" << std::endl;
            system("pause");
        }

        

    }
}
