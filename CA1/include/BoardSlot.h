#ifndef BOARD_SLOT_H
#define BOARD_SLOT_H
#include "SFML/Graphics.hpp"

struct BoardSlot
{
	BoardSlot(float t_posX, float t_posY, int t_x,int t_y) : x(t_x), y(t_y){
		body.setPosition(t_posX, t_posY);
		body.setFillColor(sf::Color::White);
		body.setRadius(10);
		body.setOutlineColor(sf::Color::Black);
		body.setOutlineThickness(2.0f);
	}
	int x, y;
	sf::CircleShape body;
	int data = 0;
};
#endif
