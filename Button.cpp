#include "Button.h"

sf::Font Font;
sf::TcpSocket Socket;

sf::Color Colors[9] = {sf::Color(230,0,32),sf::Color(244,165,130),sf::Color(247,247,247),sf::Color(146,197,222),sf::Color(5,113,176)
	,sf::Color(253,184,97),sf::Color(253,184,97),sf::Color(255,255,45),sf::Color(140,140,140)};
sf::Music Sounds[4];


int initialize()
{
	if (!Font.loadFromFile("Graphics//OxygenMono.ttf"))
        return EXIT_FAILURE;
	if(!Sounds[SOUND_PICKUP].openFromFile("Graphics//pickup.wav"))
		return EXIT_FAILURE;
	if(!Sounds[SOUND_PLACE].openFromFile("Graphics//place.wav"))
		return EXIT_FAILURE;
	if(!Sounds[SOUND_BUTTON].openFromFile("Graphics//button.wav"))
		return EXIT_FAILURE;
	if(!Sounds[SOUND_SUBMIT].openFromFile("Graphics//submit.wav"))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

Button::Button()
{
}

Button::Button(std::string txt,int x,int y)
{
	rect = sf::RectangleShape(sf::Vector2f(MenuItemWidth,MenuItemHeight));
	rect.setPosition(x,y);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(3);

	text = sf::Text(txt,Font,16);
	text.setPosition(x+TextXOffset,y+TextYOffset);
	text.setColor(sf::Color::Black);

	isDisabled = false;
}

Button::~Button()
{
}

int Button::checkCollision(int x,int y)
{
	sf::FloatRect b = rect.getGlobalBounds();
	if(x>=b.left && x<=b.left+b.width && y>=b.top && y<=b.top+b.height)
	{
		return true;
	}
	return false;
}

void Button::draw(sf::RenderWindow& window,int x,int y)
{
	if(isDisabled)
	{
		rect.setFillColor(Colors[COLOR_DISBUTTON]);
	}
	else if(checkCollision(x,y))
	{
		rect.setFillColor(Colors[COLOR_HOVERBUTTON]);
	}
	else
	{
		rect.setFillColor(Colors[COLOR_BUTTON]);
	}
	window.draw(rect);
	window.draw(text);
}

void drawTitle(sf::RenderWindow& window)
{
	//draw the title
	sf::RectangleShape rect(sf::Vector2f(LetterSize,LetterSize));
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(3);
	sf::Text txt("S",Font,16);
	std::string text = "SCRABBLE";
	txt.setColor(sf::Color::Black);
	for(int i = 0;i<8;i++)
	{
		rect.setFillColor(Colors[i]);
		rect.setPosition(TitleXOffset+LetterSize*i,TitleYOffset);
		window.draw(rect);
		txt.setPosition(TitleXOffset+LetterSize*i+TextXOffset,TitleYOffset+TextYOffset);
		txt.setString(text.at(i));
		window.draw(txt);
	}
}

void drawLetter(int x,int y,char c,bool big,sf::RenderWindow& window)
{
	sf::RectangleShape letter;
	if(big)
	{
		letter = sf::RectangleShape(sf::Vector2f(LetterSize+2,LetterSize+2));
		letter.setPosition(x-1,y-1);
	}
	else
	{
		letter = sf::RectangleShape(sf::Vector2f(LetterSize,LetterSize));
		letter.setPosition(x,y);
	}
	letter.setFillColor(Colors[COLOR_LETTER]);
	letter.setOutlineColor(sf::Color::Black);
	letter.setOutlineThickness(1);
	sf::Text txt(c,Font,16);
	txt.setColor(sf::Color::Black);
	txt.setPosition(x+TextXOffset,y+TextYOffset);
	sf::Text score(std::to_string(LetterScore[c-65]),Font,8);
	score.setPosition(x+LetterScoreXOffset,y+LetterScoreYOffset);
	score.setColor(sf::Color::Black);

	window.draw(letter);
	window.draw(txt);
	window.draw(score);
}