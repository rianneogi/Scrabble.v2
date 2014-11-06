#pragma once

#include "Button.h"

class Multiplayer
{
public:
	Multiplayer();
	~Multiplayer();

	std::string ipaddress;

	Button ReturnButton;
	Button JoinGameButton;
	Button HostGameButton;

	int handleEvent(sf::Event& event);
	void draw(sf::RenderWindow& window,int x,int y);
};

