#include "Multiplayer.h"

Multiplayer::Multiplayer()
{
	ipaddress = "";
	HostGameButton = Button("Host Game",MenuItemXOffset,NewGameYOffset);
	JoinGameButton = Button("Join Game",MenuItemXOffset,ContinueGameYOffset);
	ReturnButton = Button("Main Menu",MenuItemXOffset,ExitYOffset);
}

Multiplayer::~Multiplayer()
{
}

int Multiplayer::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonReleased)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			if(HostGameButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();

				sf::TcpListener listener;
				listener.listen(53000) != sf::Socket::Done;
				listener.accept(Socket);
				Socket.setBlocking(false);

				return GOTO_HOSTBOARD;
			}
			if(JoinGameButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();

				sf::Socket::Status status = Socket.connect(ipaddress, 53000);
				Socket.setBlocking(false);

				return GOTO_CLIENTBOARD;
			}
			if(ReturnButton.checkCollision(event.mouseButton.x,event.mouseButton.y))
			{
				Sounds[SOUND_BUTTON].play();
				return GOTO_MENU;
			}
		}
	}
	else if(event.type == sf::Event::KeyReleased)
	{
		if(event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0)
		{
			ipaddress += "0";
		}
		else if(event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
		{
			ipaddress += "1";
		}
		else if(event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
		{
			ipaddress += "2";
		}
		else if(event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3)
		{
			ipaddress += "3";
		}
		else if(event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4)
		{
			ipaddress += "4";
		}
		else if(event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5)
		{
			ipaddress += "5";
		}
		else if(event.key.code == sf::Keyboard::Num6 || event.key.code == sf::Keyboard::Numpad6)
		{
			ipaddress += "6";
		}
		else if(event.key.code == sf::Keyboard::Num7 || event.key.code == sf::Keyboard::Numpad7)
		{
			ipaddress += "7";
		}
		else if(event.key.code == sf::Keyboard::Num8 || event.key.code == sf::Keyboard::Numpad8)
		{
			ipaddress += "8";
		}
		else if(event.key.code == sf::Keyboard::Num9 || event.key.code == sf::Keyboard::Numpad9)
		{
			ipaddress += "9";
		}
		else if(event.key.code == sf::Keyboard::Period)
		{
			ipaddress += ".";
		}
		else if(event.key.code == sf::Keyboard::Delete || event.key.code == sf::Keyboard::BackSpace)
		{
			if(ipaddress.size()>0)
				ipaddress.erase(ipaddress.size()-1);
		}
	}
	return GOTO_NONE;
}

void Multiplayer::draw(sf::RenderWindow& window,int x,int y)
{
	drawTitle(window);

	sf::Text txt(ipaddress,Font,16);
	txt.setColor(sf::Color::Black);

	/*txt.setString("Your IP: "+Socket.getRemoteAddress().toString());
	txt.setPosition(MenuItemXOffset,MultiplayerYOffset - 10);
	window.draw(txt);*/

	txt.setString("Enter IP: "+ipaddress);
	txt.setPosition(MenuItemXOffset,MultiplayerYOffset);
	window.draw(txt);

	HostGameButton.draw(window,x,y);
	JoinGameButton.draw(window,x,y);
	ReturnButton.draw(window,x,y);
}

