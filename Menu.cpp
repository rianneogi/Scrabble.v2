#include "Menu.h"


Menu::Menu()
{
	SinglePlayerButton = Button("Single Player",MenuItemXOffset,NewGameYOffset);
	ContinueButton = Button("Continue Game",MenuItemXOffset,ContinueGameYOffset);
	MultiplayerButton = Button("Multiplayer",MenuItemXOffset,MultiplayerYOffset);
	ExitButton = Button("Exit",MenuItemXOffset,ExitYOffset);
}


Menu::~Menu()
{
}

int Menu::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonReleased)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			if(SinglePlayerButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_NEWBOARD;
			}
			if(ContinueButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_BOARD;
			}
			if(MultiplayerButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_MULTIPLAYER;
			}
			if(ExitButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_EXIT;
			}
		}
	}
	return GOTO_NONE;
}

void Menu::draw(sf::RenderWindow& window,int x,int y)
{
	drawTitle(window);

	SinglePlayerButton.draw(window,x,y);
	ContinueButton.draw(window,x,y);
	MultiplayerButton.draw(window,x,y);
	ExitButton.draw(window,x,y);
}

