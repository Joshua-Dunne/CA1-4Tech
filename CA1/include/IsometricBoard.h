#pragma once
#ifndef ISOMETRIC_BOARD_H
#define ISOMETRIC_BOARD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Global.h"

class IsometricBoard
{
public:
	IsometricBoard();

	void setupBoard();
	void update(sf::Time& t_dt);
	void input(sf::Event t_event);
	void render(sf::RenderWindow& t_window);

private:


};
#endif
