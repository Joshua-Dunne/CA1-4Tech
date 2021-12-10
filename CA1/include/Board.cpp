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
    if (!m_gameFinished)
    {
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
    }
    else
    {
        if (m_boardCounter < 16)
            std::cout << "Player " << m_currentPlayer << " wins!";
        else if (!m_gameWon)
            std::cout << "Draw between Player 1 and Player 2!";
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
                    endCheck();
                    if (!m_gameFinished) // don't set the new player turn if the game is over
                        m_currentPlayer = 2;

                    break;
                case 2:
                    m_boardData[m_row][m_column] = 2; // assigns the player's piece into the board
                    endCheck();
                    if (!m_gameFinished) // don't set the new player turn if the game is over
                        m_currentPlayer = 1;
                    break;
                default:
                    std::cout << "error choosing player piece to place, defaulting to player 1 placing piece" << std::endl;
                    m_boardData[m_row][m_column] = 1; // assigns the player's piece into the board
                    endCheck();
                    m_currentPlayer = 2;
                }

                m_boardCounter++;
                endCheck();
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

void Board::boardEvaluation()
{

    // assign the weight to the grid

    // update the weight of the grid
    // after the player places his move.





}

void Board::endCheck()
{
    if (!m_gameFinished)
    {
        bool gameWon = false;
        // check all rows to see if a win is made
        if (m_boardCounter < 16)
        {
            for (int i = 0; i < 4; i++)
            {
                if (m_boardData[i][0] == m_currentPlayer && m_boardData[i][1] == m_currentPlayer
                    && m_boardData[i][2] == m_currentPlayer && m_boardData[i][3] == m_currentPlayer && !m_gameFinished)
                {
                    m_gameFinished = true;
                    m_gameWon = true;
                    break;
                }

                if (m_boardData[0][i] == m_currentPlayer && m_boardData[1][i] == m_currentPlayer
                    && m_boardData[2][i] == m_currentPlayer && m_boardData[3][i] == m_currentPlayer && !m_gameFinished)
                {
                    m_gameFinished = true;
                    m_gameWon = true;
                    break;
                }
            }

            if (!m_gameFinished)
            { // only do corner checks if the game isn't over already
                if (m_boardData[0][3] == m_currentPlayer && m_boardData[1][2] == m_currentPlayer
                    && m_boardData[2][1] == m_currentPlayer && m_boardData[3][0] == m_currentPlayer && !m_gameFinished)
                {
                    m_gameFinished = true;
                    m_gameWon = true;
                } 
                else if (m_boardData[0][0] == m_currentPlayer && m_boardData[1][1] == m_currentPlayer
                    && m_boardData[2][2] == m_currentPlayer && m_boardData[3][3] == m_currentPlayer && !m_gameFinished)
                {
                    m_gameFinished = true;
                    m_gameWon = true;
                }
            }
        }
        else if (m_boardCounter >= 16)
        {
            m_gameFinished = true;
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

    m_gameFinished = false;
    m_gameWon = false;
    m_boardCounter = 0;

    // randomly pick a new player
    m_currentPlayer = (rand() % 2) + 1;
}
