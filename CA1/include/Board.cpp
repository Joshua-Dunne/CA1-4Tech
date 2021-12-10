#include "Board.h"

Board::Board()
{
    srand(static_cast<unsigned int>(time(NULL)));
}

void Board::update()
{
}

void Board::render()
{
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
}

bool Board::input(int t_player, int t_row, int t_col)
{
    if (t_row >= 0 && t_row < 4 && t_col >= 0 && t_col < 4)
    {
        if (m_boardData[t_row][t_col] != 1 && m_boardData[t_row][t_col] != 2)
        { // check to make sure that the current info trying to be placed doesn't exist already
            // swap current player
            m_boardData[t_row][t_col] = t_player;
            return true;
        }
        else
        {
            std::cout << "Error! A" << m_row << ", B" << m_column << " already occupied!" << std::endl;
            system("pause");
            return false;
        }
    }
    else
    {
        std::cout << "Error input 0-3 only for rows and columns" << std::endl;
        system("pause");
        return false;
    }

    // return false as a catch all in case something goes wrong
    return false;
}

void Board::endCheck()
{
    if (!m_boardFinished)
    {
        // check all rows to see if a win is made
        if (m_boardCounter < 16)
        {
            for (int i = 0; i < 4; i++)
            {
                if (m_boardData[i][0] == m_currentPlayer && m_boardData[i][1] == m_currentPlayer
                    && m_boardData[i][2] == m_currentPlayer && m_boardData[i][3] == m_currentPlayer && !m_boardFinished)
                {
                    m_boardFinished = true;
                    m_gameWon = true;
                    break;
                }

                if (m_boardData[0][i] == m_currentPlayer && m_boardData[1][i] == m_currentPlayer
                    && m_boardData[2][i] == m_currentPlayer && m_boardData[3][i] == m_currentPlayer && !m_boardFinished)
                {
                    m_boardFinished = true;
                    m_gameWon = true;
                    break;
                }
            }

            if (!m_boardFinished)
            { // only do corner checks if the game isn't over already
                if (m_boardData[0][3] == m_currentPlayer && m_boardData[1][2] == m_currentPlayer
                    && m_boardData[2][1] == m_currentPlayer && m_boardData[3][0] == m_currentPlayer && !m_boardFinished)
                {
                    m_boardFinished = true;
                    m_gameWon = true;
                } 
                else if (m_boardData[0][0] == m_currentPlayer && m_boardData[1][1] == m_currentPlayer
                    && m_boardData[2][2] == m_currentPlayer && m_boardData[3][3] == m_currentPlayer && !m_boardFinished)
                {
                    m_boardFinished = true;
                    m_gameWon = true;
                }
            }
        }
        else if (m_boardCounter >= 16)
        {
            m_boardFinished = true;
            m_gameWon = false;
        }
    }
}

void Board::reset()
{
    system("pause");

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_boardData[i][j] = 0;
        }
    }

    m_boardFinished = false;
    m_gameWon = false;
    m_boardCounter = 0;

    // randomly pick a new player
    m_currentPlayer = (rand() % 2) + 1;
}
