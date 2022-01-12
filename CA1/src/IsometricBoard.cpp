#include "../include/IsometricBoard.h"

IsometricBoard::IsometricBoard()
{
	setupBoard();
}

void IsometricBoard::setupBoard()
{
	// sets the board
	float tempX = 0.0f;
	float tempY = 0.0f;
	int count = 0;
	int mod = 16;
	for (int i = 0; i < 64; i++) {
		m_circleSlots.push_back(createCircles(tempX, tempY));
		tempX += 20;
		count++;
		if (tempX >= 80)
		{
			tempX = 0;
			tempY += 20;
			count = count % mod;
			if (count == 0)
			{
				tempY += 40;
			}
		}
	}
}

sf::CircleShape IsometricBoard::createCircles(float t_x, float t_y)
{
	m_circles.setRadius(10);
	m_circles.setPosition(t_x, t_y);
	m_circles.setFillColor(sf::Color::White);
	m_circles.setOutlineColor(sf::Color::Black);
	m_circles.setOutlineThickness(2.0f);

	return m_circles;
}

void IsometricBoard::update(sf::Time& t_dt)
{
	// updates the iso board
}


void IsometricBoard::input(sf::Event t_event)
{
	// for mouse clicks
}

void IsometricBoard::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_circleSlots.size(); i++)
	{
		t_window.draw(m_circleSlots[i]);
	}

}
