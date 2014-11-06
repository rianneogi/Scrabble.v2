#pragma once

#include "Menu.h"
#include "Board.h"
#include "Multiplayer.h"

enum GameStates {STATE_MENU,STATE_BOARD,STATE_MULTIPLAYER};

class Game
{
public:
	Game();
	~Game();

	int MouseX,MouseY;

	int GameState;
	Menu menu;
	Multiplayer multiplayer;
	Board board;

	sf::RectangleShape background;

	void receivePacket(sf::Packet& p);
	int handleEvent(sf::Event& event);
	void draw(sf::RenderWindow& window);
};

