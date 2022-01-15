#pragma once
#ifndef ISOMETRIC_BOARD_H
#define ISOMETRIC_BOARD_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"
#include "BoardSlot.h"

class IsometricBoard
{
public:
	IsometricBoard(sf::RenderWindow& t_window);

	inline void getBoards(Board& t_board) { m_board = &t_board; };

	void setupBoard();
	sf::CircleShape createCircles(float t_x, float t_y);
	void update();
	bool input(sf::Event t_event, int t_input);
	void render(sf::RenderWindow& t_window);
	int m_count = 0;

private:
	std::vector<BoardSlot> m_circleSlots;
	sf::CircleShape m_circles;
	Board* m_board;
	sf::Text m_text;
	sf::Font m_font;
	sf::RenderWindow& m_window;

};
#endif
