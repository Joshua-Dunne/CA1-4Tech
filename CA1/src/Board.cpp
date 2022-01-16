#include "../include/Board.h"

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
    for (int i = 0; i < 16; i++)
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
            m_boardCounter++;
            return true;

        }
        else
        {
            std::cout << "Error! A" << t_row << ", B" << t_col << " already occupied!" << std::endl;
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

void Board::endCheck(int t_currPlayer)
{
    if (!m_boardFinished)
    {
        // check all rows to see if a win is made
        if (m_boardCounter < 16)
        {
            for (int i = 0; i < 16; i++)
            {
                if (m_boardData[i][0] == t_currPlayer && m_boardData[i][1] == t_currPlayer
                    && m_boardData[i][2] == t_currPlayer && m_boardData[i][3] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                    break;
                }

                if (m_boardData[0][i] == t_currPlayer && m_boardData[1][i] == t_currPlayer
                    && m_boardData[2][i] == t_currPlayer && m_boardData[3][i] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                    break;
                }
            }

            for (int i = 0; i < 4; i++)
            {
                // checks for the 3d row if the game isn't over already
                if (m_boardData[i][0] == t_currPlayer && m_boardData[i+4][1] == t_currPlayer
                    && m_boardData[i+8][2] == t_currPlayer && m_boardData[i+12][3] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                    break;
                }
                
                // checks for the 3d column if the game isn't over
                if (m_boardData[0][i] == t_currPlayer && m_boardData[5][i] == t_currPlayer
                    && m_boardData[10][i] == t_currPlayer && m_boardData[15][i] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                    break;
                }
            }

            if (!m_boardFinished)
            { // only do corner checks if the game isn't over already
                if (m_boardData[0][3] == t_currPlayer && m_boardData[1][2] == t_currPlayer
                    && m_boardData[2][1] == t_currPlayer && m_boardData[3][0] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                } 
                else if (m_boardData[0][0] == t_currPlayer && m_boardData[1][1] == t_currPlayer
                    && m_boardData[2][2] == t_currPlayer && m_boardData[3][3] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                }
            }

            if (!m_boardFinished)
            { // only 3d corner checks if the game isn't over already
                if (m_boardData[0][3] == t_currPlayer && m_boardData[5][2] == t_currPlayer
                    && m_boardData[10][1] == t_currPlayer && m_boardData[15][0] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                }
                else if (m_boardData[0][0] == t_currPlayer && m_boardData[5][1] == t_currPlayer
                    && m_boardData[10][2] == t_currPlayer && m_boardData[15][3] == t_currPlayer)
                {
                    m_boardFinished = true;
                    m_boardWin = true;
                }
            }



        }
        else
        { // if the board counter is 16 or higher, the board is full
            m_boardFinished = true;
        }
    }
}

void Board::reset()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_boardData[i][j] = 0;
        }
    }

    m_boardFinished = false;
    m_boardWin = false;
    m_boardCounter = 0;
}




std::vector<std::pair<int,int>> Board::getVaildMoves()
{
    std::vector<std::pair<int, int>> possibleMoves;


    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_boardData[i][j] == 0) // checks for empty spaces
            {
                possibleMoves.push_back(std::make_pair(i, j)); // sets the empty space
            }
        }
    }
    return possibleMoves;
}
