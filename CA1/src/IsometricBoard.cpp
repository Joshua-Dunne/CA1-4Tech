#include "../include/IsometricBoard.h"

IsometricBoard::IsometricBoard(sf::RenderWindow& t_window) : m_window(t_window)
{
	if (!m_font.loadFromFile("assets/fonts/ariblk.ttf"))
	{
		//error
		std::cout << "I don't where the files for this" << std::endl;

	}
	m_text.setFont(m_font);
	m_text.setFillColor(sf::Color::White);
	m_text.setCharacterSize(16);

	setupBoard(); // sets the circle
}

/// <summary>
/// Sets the screen for the game of 4-tac
/// </summary>
void IsometricBoard::setupBoard()
{
	// sets the board
	float tempX = 0.0f; // x position
	float tempY = 20.0f; // y position
	int count = 0; // the amount of circles
	int mod = 16; // 4*4 of the board is 16
	for (int i = 0; i < 16; i++) {
		if (tempX >= 100)
		{
			tempX = 0; // reset the x-axis
			tempY += 25; // add the new y-axis for the new circle
			count = count % mod; // checks if there's 16 circles in the board
			if (count == 0)
			{
				tempY += 40; // Creates a new gap for the new board
			}
		}

		for (int j = 0; j < 4; j++)
		{
			m_circleSlots.push_back(BoardSlot(tempX, tempY,i,j)); // creates the circle and stored into the vector
			tempX += 25; // add the new x-position for the next circle
			count++; // up the count of circle
		}

	}
}

/// <summary>
/// the board update
/// </summary>
/// <param name="t_board">What board its updating</param>
void IsometricBoard::update(sf::Time dt)
{
	// updates the iso board
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_board->m_boardData[i][j] == 1)
			{
				m_circleSlots[m_count].body.setFillColor(sf::Color::Red);
			}
			else if (m_board->m_boardData[i][j] == 2)
			{
				m_circleSlots[m_count].body.setFillColor(sf::Color::Yellow);
			}
			else {
				m_circleSlots[m_count].body.setFillColor(sf::Color::White);
			}
			m_count++;
		}
	}

	m_timer -= dt.asSeconds();


	
}

/// <summary>
/// For button and mouse actions
/// </summary>
/// <param name="t_event">The event action</param>
bool IsometricBoard::input(sf::Event t_event, int t_input)
{
	if (!m_board->m_boardFinished) {
		// for mouse clicks or keyboard input
		if (t_event.type == sf::Event::MouseButtonPressed)
		{
			for (int i = 0; i < m_circleSlots.size(); i++) {
				if (m_circleSlots[i].body.getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
				{
					if (m_board->m_boardData[m_circleSlots[i].x][m_circleSlots[i].y] == 0)
					{
						m_board->m_boardData[m_circleSlots[i].x][m_circleSlots[i].y] = t_input;
					}
					else {
						m_error = true;
						m_timer = 3.0f;
						break;
					}
					return true;
				}
			}
		}
		return false;
	}
}


/// <summary>
/// The render is what it says, IT RENDERS
/// </summary>
/// <param name="t_window">Passing the window for it to draw the object</param>
void IsometricBoard::render(sf::RenderWindow& t_window)
{
	float temp = 0;
	for (int i = 0; i < m_circleSlots.size(); i++)
	{	
		t_window.draw(m_circleSlots[i].body);
	}

	for (int i = 0; i < 4; i++)
	{
		m_text.setString("Board " + std::to_string(i+1));
		m_text.setPosition(20,temp);
		t_window.draw(m_text);
		temp += 140.0f;
	}

	m_text.setString("Select a board");
	m_text.setPosition(400, 0);
	t_window.draw(m_text);
	m_text.setString("Select a Row");
	m_text.setPosition(400, 20);
	t_window.draw(m_text);
	m_text.setString("Select a Col");
	m_text.setPosition(400, 40);
	t_window.draw(m_text);

	if (m_error)
	{
		if (m_timer < 0.0f)
		{
			m_error = false;
		}
		else {
			m_text.setString("You can't place a piece there\nits already occupied");
			m_text.setPosition(400.0f, 500.0f);
			t_window.draw(m_text);
		}
	}


}