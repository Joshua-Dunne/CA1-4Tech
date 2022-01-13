#include "../include/IsometricBoard.h"

IsometricBoard::IsometricBoard()
{
	setupBoard(); // sets the circle
}

/// <summary>
/// Sets the screen for the game of 4-tac
/// </summary>
void IsometricBoard::setupBoard()
{
	// sets the board
	float tempX = 0.0f; // x position
	float tempY = 0.0f; // y position
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
void IsometricBoard::update(int t_board)
{
	// updates the iso board
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_board[t_board].m_boardData[i][j] == 1)
			{
				m_circleSlots[m_count].setFillColor(sf::Color::Red);
			}
			else if (m_board[t_board].m_boardData[i][j] == 2)
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
}


/// <summary>
/// The render is what it says, IT RENDERS
/// </summary>
/// <param name="t_window">Passing the window for it to draw the object</param>
void IsometricBoard::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_circleSlots.size(); i++)
	{	
		t_window.draw(m_circleSlots[i]);
	}
}