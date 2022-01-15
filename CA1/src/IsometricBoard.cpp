#include "../include/IsometricBoard.h"

IsometricBoard::IsometricBoard(sf::RenderWindow& t_window) : m_window(t_window)
{
	if (!m_font.loadFromFile("assets/fonts/ariblk.ttf"))
	{
		//error

	}

	m_text.setFillColor(sf::Color::White);
	m_text.setFont(m_font);
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
	for (int i = 0; i < 64; i++) {
		m_circleSlots.push_back(createCircles(tempX, tempY)); // creates the circle and stored into the vector
		tempX += 25; // add the new x-position for the next circle
		count++; // up the count of circle
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
	}
}

/// <summary>
/// Creation of circles for the vector to hold
/// </summary>
/// <param name="t_x">X position</param>
/// <param name="t_y">Y position</param>
/// <returns></returns>
sf::CircleShape IsometricBoard::createCircles(float t_x, float t_y)
{
	// setup of the circles
	m_circles.setRadius(10);
	m_circles.setPosition(t_x, t_y);
	m_circles.setFillColor(sf::Color::White);
	m_circles.setOutlineColor(sf::Color::Black);
	m_circles.setOutlineThickness(2.0f);

	return m_circles;
}

/// <summary>
/// the board update
/// </summary>
/// <param name="t_board">What board its updating</param>
void IsometricBoard::update()
{
	// updates the iso board
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_board.m_boardData[i][j] == 1)
			{
				m_circleSlots[m_count].setFillColor(sf::Color::Red);
			}
			else if (m_board.m_boardData[i][j] == 2)
			{
				m_circleSlots[m_count].setFillColor(sf::Color::Yellow);
			}
			else {
				m_circleSlots[m_count].setFillColor(sf::Color::White);
			}
			m_count++;
		}
	}
	
}

/// <summary>
/// For button and mouse actions
/// </summary>
/// <param name="t_event">The event action</param>
void IsometricBoard::input(sf::Event t_event)
{
	// for mouse clicks or keyboard input
	if (t_event.type == sf::Event::MouseButtonPressed)
	{
		for (int i = 0; i < m_circleSlots.size(); i++) {
			if (m_circleSlots[i].getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
			{
				std::cout << "you have picked on slot " << i << std::endl;
			}
		}
	}
}


/// <summary>
/// The render is what it says, IT RENDERS
/// </summary>
/// <param name="t_window">Passing the window for it to draw the object</param>
void IsometricBoard::render(sf::RenderWindow& t_window)
{
	int temp = 0;
	for (int i = 0; i < m_circleSlots.size(); i++)
	{	
		t_window.draw(m_circleSlots[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		m_text.setString("Board " + std::to_string(i+1));
		m_text.setPosition(20,temp);
		t_window.draw(m_text);
		temp += 140;
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
}