#include "../include/Game.h"

Game::Game() : m_window(sf::VideoMode(800u, 600u), "Lab1")
{
	//m_window.setFramerateLimit(60u);

}

Game::~Game()
{
}

/// <summary>
/// Run the main Game loop
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	while (m_window.isOpen())
	{
		processInput();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
		render();
	}
}

/// <summary>
/// Processes inputs from Events
/// </summary>
void Game::processInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}

	}
}

void Game::update(sf::Time& dt)
{
	if (!m_gameFinished)
	{
		getInput(dt);
		checkBoards(dt);

		// alternate between players
		// we alternate afterwards so board checks output the correct current player
		if (m_currentPlayer == 1)
			m_currentPlayer = 2;
		else
			m_currentPlayer = 1;
	}
	else
	{
		system("pause");
		resetGame();
	}

}

void Game::getInput(sf::Time& dt)
{
	int board = 0;
	int row = 0;
	int col = 0;
	
	while (board < 1 || board > 4)
	{
		render();
		std::cout << "Select Board: ";
		std::cin >> board;
		std::cout << "Select row: A";
		std::cin >> row;
		std::cout << "Select row: B";
		std::cin >> col;

#ifndef DEBUG

		// if the board entered is 9,
		// fill all the boards up to the brim with no winner
		if (board == 9)
		{
			for (int i = 0; i < 4; i++)
			{
				m_boards[i].boardFill();
				m_boards[i].m_boardCounter = 14;
			}
		}
#endif

		if (board > 4 || board < 1)
		{
			std::cout << "Incorrect number entered for board!" << std::endl;
			system("pause");
			continue;
		}

		if (m_boards[board - 1].input(m_currentPlayer, row, col))
		{
			m_vaildMoves = m_boards[board - 1].getVaildMoves(); // gives the possible moves of that board
			break;
		}
		else
		{
			board = 0;
			continue;
		}
	}
}

void Game::checkBoards(sf::Time& dt)
{
	int boardsFinished = 0;

	// we will check to see if any board has a win
	for (int i = 0; i < 4; i++)
	{
		m_boards[i].endCheck(m_currentPlayer);

		// if the board is at 16 plays
		// and the board has not detected a win
		// then count it as a dead board and continue on
		if (m_boards[i].m_boardCounter >= 16 && !m_boards[i].m_boardWin && m_boards[i].m_boardFinished)
		{
			boardsFinished++;
			continue;
		}

		if (m_boards[i].m_boardFinished)
		{
			// if the board has detected a win
			// the current player has won
			if (m_boards[i].m_boardWin)
			{
				m_gameFinished = true;
				m_gameWon = true;
				break; // since a board has reached an end state, we can jump out now
			}
		}
	}
	

	// at this point, we will check to see if the game is a draw
	// since we count how many boards are complete,
	// but have not resulted in a win,
	// we can use that here to detect a draw
	if (boardsFinished == 4)
	{
		m_gameFinished = true;
		render();
		std::cout << "Draw between players!" << std::endl;
	}
	else if (m_gameWon)
	{
		m_gameFinished = true;
		render();
		std::cout << "Player " << m_currentPlayer << " has won!" << std::endl;	
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	system("cls");
	for (int i = 0; i < 4; i++)
	{
		std::cout << "Board " << i + 1 << ": " << std::endl;
		m_boards[i].render();
	}

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

		std::cout << std::endl;
	}

	m_window.display();
}

void Game::resetGame()
{
	for (int i = 0; i < 4; i++)
	{
		m_boards[i].reset();
	}

	// randomly pick a new player
	m_currentPlayer = (rand() % 2) + 1;

	m_gameFinished = false;
	m_gameWon = false;
}
