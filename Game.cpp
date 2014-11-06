#include "Game.h"


Game::Game()
{
	board = Board();
	menu = Menu();
	multiplayer = Multiplayer();
	GameState = STATE_MENU;
	background = sf::RectangleShape(sf::Vector2f(WindowWidth,WindowHeight));
	background.setPosition(0,0);
	background.setFillColor(sf::Color::White);

	MouseX = 0;
	MouseY = 0;
}

Game::~Game()
{
}

int Game::handleEvent(sf::Event& event)
{
	int r = -1;
	if(GameState==STATE_BOARD)
	{
		r = board.handleEvent(event);
	}
	else if(GameState==STATE_MULTIPLAYER)
		r = multiplayer.handleEvent(event);
	else if(GameState==STATE_MENU)
		r = menu.handleEvent(event);
	if(r==GOTO_MENU)
	{
		GameState = STATE_MENU;
	}
	else if(r==GOTO_BOARD)
	{
		GameState = STATE_BOARD;
	}
	else if(r==GOTO_MULTIPLAYER)
	{
		GameState = STATE_MULTIPLAYER;
	}
	else if(r==GOTO_NEWBOARD)
	{
		board = Board();
		GameState = STATE_BOARD;
	}
	else if(r==GOTO_HOSTBOARD)
	{
		board = Board();
		board.amIHost = true;
		board.myPlayer = 0;
		board.isMultiplayer = true;
		GameState = STATE_BOARD;
	}
	else if(r==GOTO_CLIENTBOARD)
	{
		board = Board();
		board.amIHost = false;
		board.myPlayer = 1;
		board.isMultiplayer = true;
		GameState = STATE_BOARD;
	}
	else if(r==GOTO_EXIT)
	{
		return -1;
	}

	if(event.type==sf::Event::MouseMoved)
	{
		MouseX = event.mouseMove.x;
		MouseY = event.mouseMove.y;
	}
	return r;
}

void Game::draw(sf::RenderWindow& window)
{
	window.draw(background);
	if(GameState==STATE_BOARD)
		board.draw(window,MouseX,MouseY);
	else if(GameState==STATE_MULTIPLAYER)
		multiplayer.draw(window,MouseX,MouseY);
	else if(GameState==STATE_MENU)
		menu.draw(window,MouseX,MouseY);
}

void Game::receivePacket(sf::Packet& p)
{
	board.receivePacket(p);
}
