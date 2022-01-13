#pragma once
#ifndef ISOMETRIC_BOARD_H
#define ISOMETRIC_BOARD_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"
#include <algorithm>



class IsometricBoard
{
public:
	IsometricBoard();

	inline void getBoards(std::vector<Board>& t_boards) { m_board = t_boards; };

	void setupBoard();
	sf::CircleShape createCircles(float t_x, float t_y);
	void update(sf::Time& t_dt, int t_board);
	void input(sf::Event t_event);
	void render(sf::RenderWindow& t_window);
	std::vector<sf::CircleShape> getCircleSlots();
	int m_count = 0;

private:
	std::vector<sf::CircleShape> m_circleSlots;
	sf::CircleShape m_circles;
	std::vector<Board> m_board;

};
#endif
