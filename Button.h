#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

#include "constants.h"

extern sf::Font Font;
extern int LetterScore[26];
extern sf::TcpSocket Socket;

enum COLOR {COLOR_TRIPLEWORD,COLOR_DOUBLEWORD,COLOR_NORMAL,COLOR_DOUBLELETTER,COLOR_TRIPLELETTER,COLOR_LETTER,COLOR_BUTTON,COLOR_HOVERBUTTON,COLOR_DISBUTTON};
enum SOUND {SOUND_BUTTON,SOUND_PICKUP,SOUND_PLACE,SOUND_SUBMIT};
enum GOTO {GOTO_NONE,GOTO_BOARD,GOTO_NEWBOARD,GOTO_MULTIPLAYER,GOTO_EXIT,GOTO_MENU,GOTO_HOSTBOARD,GOTO_CLIENTBOARD};
enum NETWORKMESSAGE {MESSAGE_SUBMIT,MESSAGE_SHUFFLE,MESSAGE_PLACETILE,MESSAGE_PASSTURN};

extern sf::Color Colors[9];
extern sf::Music Sounds[4];

class Button
{
public:
	Button();
	Button(std::string txt,int x,int y);
	~Button();

	sf::RectangleShape rect;
	sf::Text text;
	bool isDisabled;

	int checkCollision(int x,int y);
	void draw(sf::RenderWindow& window,int x,int y);
};

int initialize();
void drawTitle(sf::RenderWindow& window);
void drawLetter(int x,int y,char c,bool big,sf::RenderWindow& window);

