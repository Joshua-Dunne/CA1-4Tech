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
		for (int i = 0; i < 4; i++)
		{
			m_boards[i].reset();
		}
	}

}

void Game::getInput(sf::Time& dt)
{
	int board = 0;
	int row;
	int col;
	
	while (board < 1 || board > 4)
	{
		render();
		std::cout << "Select Board: ";
		std::cin >> board;
		std::cout << "Select row: A";
		std::cin >> row;
		std::cout << "Select row: B";
		std::cin >> col;

		if (board > 4 || board < 1)
		{
			std::cout << "Incorrect number entered for board!" << std::endl;
			system("pause");
			continue;
		}

		if (m_boards[board - 1].input(m_currentPlayer, row, col))
		{
			break;
		}
		else
		{
			board = 0;
			continue;
		}
	}
	m_boardCount++;
}

void Game::checkBoards(sf::Time& dt)
{

	// once the count hits 64, all boards should be full
	// we can do a final check to make sure no player has won,
	// before we decide to default
	if (m_boardCount >= 64)
	{
		/*
		if (...)
		{

		}
		else
		{
			std::cout << "Draw between players!" << std::endl;
			m_gameFinished = true;
			system("pause");
		}*/
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	//m_board.render();

	system("cls");
	for (int i = 0; i < 4; i++)
	{
		std::cout << "Board " << i + 1 << ": " << std::endl;
		m_boards[i].render();
	}

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

	m_window.display();
}
