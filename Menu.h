#pragma once

#include "Button.h"

class Menu
{
public:
	Menu();
	~Menu();

	Button SinglePlayerButton;
	Button ContinueButton;
	Button MultiplayerButton;
	Button ExitButton;

	int handleEvent(sf::Event& event);
	void draw(sf::RenderWindow& window,int x,int y);
};

