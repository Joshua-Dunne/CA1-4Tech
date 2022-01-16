#include "../include/Game.h"

Game::Game() : m_window(sf::VideoMode(800u, 600u), "Lab1"), m_isoBoard(m_window)
{
	//m_window.setFramerateLimit(60u);
	// add the 4 boards

	//aiPlayer = new AI(2, m_board);

	m_isoBoard.getBoards(m_board);
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

		if(!m_playMade && !m_gameFinished)
			m_playMade = m_isoBoard.input(event,m_currentPlayer);
	}
}

void Game::update(sf::Time& dt)
{
	checkBoards(dt);
	if (!m_gameFinished)
	{
		if (m_playMade)
		{
			if (m_currentPlayer == 1)
				m_currentPlayer = 2;
			else
				m_currentPlayer = 1;
			m_playMade = false;
		}

		if (m_currentPlayer == 2)
		{
			//aiPlayer->makePlay();
			//m_isoBoard.getBoards(m_board); // gets the recent moves
		}

		// alternate between players
		// we alternate afterwards so board checks output the correct current player

		m_isoBoard.update(dt); // updates the board
		m_isoBoard.m_count = 0; // resets the count of pieces
	}
	else
	{
		resetGame(); // resets the game board
		m_isoBoard.update(dt); // then update the board screen
		m_isoBoard.m_count = 0; // reset the cout of pieces
	}
	m_board.update(dt); // to get the timer working

}

void Game::checkBoards(sf::Time& dt)
{
	int boardsFinished = 0;

	// we will check to see if any board has a win

	m_board.endCheck(m_currentPlayer);

	if (m_board.m_boardFinished)
	{
		// if the board has detected a win
		// the current player has won
		if (m_board.m_boardWin)
		{
			m_gameFinished = true;
			m_gameWon = true;
		}
	}
	// at this point, we will check to see if the game is a draw
	// since we count how many boards are complete,
	// but have not resulted in a win,
	// we can use that here to detect a draw
	if (boardsFinished == 4)
	{
		m_gameFinished = true;
	}
	else if (m_gameWon)
	{
		m_gameFinished = true;
	}
}


void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_isoBoard.render(m_window); // renders the board into the screen
	m_board.render(m_window);
	m_window.display();
}

void Game::resetGame()
{
	m_board.reset();
	m_isoBoard.getBoards(m_board); // gets the piece positions
	// randomly pick a new player
	m_currentPlayer = (rand() % 2) + 1;

	m_gameFinished = false;
	m_gameWon = false;
}
