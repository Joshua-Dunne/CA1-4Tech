#pragma once
#ifndef ISOMETRIC_BOARD_H
#define ISOMETRIC_BOARD_H

#include <iostream>
#include <SFML/Graphics.hpp>


class IsometricBoard
{
public:
	IsometricBoard();

	void setupBoard();
	sf::CircleShape createCircles(float t_x, float t_y);
	void update(sf::Time& t_dt);
	void input(sf::Event t_event);
	void render(sf::RenderWindow& t_window);

private:
	std::vector<sf::CircleShape> m_circleSlots;
	sf::CircleShape m_circles;

};
#endif
