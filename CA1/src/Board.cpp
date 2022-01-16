#include "../include/Board.h"

Board::Board()
{
    srand(static_cast<unsigned int>(time(NULL)));

    if (!m_font.loadFromFile("assets/fonts/ariblk.ttf"))
    {
        //error
        std::cout << "I don't where the files for this" << std::endl;

    }
    m_winnerText.setFont(m_font);
    m_winnerText.setFillColor(sf::Color::White);
    m_winnerText.setCharacterSize(16);
    m_winnerText.setPosition(400.0f, 500.0f);
}

void Board::update(sf::Time dt)
{
    m_timer -= dt.asSeconds();
}

void Board::render(sf::RenderWindow& t_window)
{
    if (m_winTextCheck) {
        if (m_timer < 0.0f)
        {
            m_winTextCheck = false;
        }
        else
            t_window.draw(m_winnerText);
    }
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
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                    break;
                }

                if (m_boardData[0][i] == t_currPlayer && m_boardData[1][i] == t_currPlayer
                    && m_boardData[2][i] == t_currPlayer && m_boardData[3][i] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                    break;
                }
            }

            for (int i = 0; i < 4; i++)
            {
                // checks for the 3d row if the game isn't over already
                if (m_boardData[i][0] == t_currPlayer && m_boardData[i + 4][1] == t_currPlayer
                    && m_boardData[i + 8][2] == t_currPlayer && m_boardData[i + 12][3] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                    break;
                }

                // checks for the 3d column if the game isn't over
                if (m_boardData[0][i] == t_currPlayer && m_boardData[5][i] == t_currPlayer
                    && m_boardData[10][i] == t_currPlayer && m_boardData[15][i] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                    m_timer = 3.0f;
                    break;
                }
            }

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    // checks for the 3d row top to bottom if the game isn't over already
                    if (m_boardData[i + 0][j] == t_currPlayer && m_boardData[i + 4][j] == t_currPlayer
                        && m_boardData[i + 8][j] == t_currPlayer && m_boardData[i + 12][j] == t_currPlayer)
                    {
                        m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                        m_boardFinished = true;
                        m_boardWin = true;
                        m_winTextCheck = true;
                        break;
                    }
                }
            }

            if (!m_boardFinished)
            { // only do corner checks if the game isn't over already
                if (m_boardData[0][3] == t_currPlayer && m_boardData[1][2] == t_currPlayer
                    && m_boardData[2][1] == t_currPlayer && m_boardData[3][0] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                }
                else if (m_boardData[0][0] == t_currPlayer && m_boardData[1][1] == t_currPlayer
                    && m_boardData[2][2] == t_currPlayer && m_boardData[3][3] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                }
            }

            if (!m_boardFinished)
            { // only 3d corner checks if the game isn't over already
                if (m_boardData[0][3] == t_currPlayer && m_boardData[5][2] == t_currPlayer
                    && m_boardData[10][1] == t_currPlayer && m_boardData[15][0] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                }
                else if (m_boardData[0][0] == t_currPlayer && m_boardData[5][1] == t_currPlayer
                    && m_boardData[10][2] == t_currPlayer && m_boardData[15][3] == t_currPlayer)
                {
                    m_winnerText.setString("Player " + std::to_string(t_currPlayer) + " has won\nClick again to play");
                    m_boardFinished = true;
                    m_boardWin = true;
                    m_winTextCheck = true;
                }
            }
        }
        else
        { // if the board counter is 16 or higher, the board is full
            m_boardFinished = true;
            m_winTextCheck = true;
            m_winnerText.setString("Game has end in a draw\nClick again to play");
        }
    }

    if (m_winTextCheck)
    {
        m_timer = 3.0f;
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
