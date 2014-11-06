#include "Game.h"

int main()
{
	srand(time(0));

	initialize();

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Scrabble");
	
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i(0,0));

	// Load a music to play
    sf::Music music;
    if (!music.openFromFile("Graphics//song_game.ogg"))
        return EXIT_FAILURE;
	music.setLoop(true);
	music.setVolume(100);
	music.play();

	Game g;
	sf::Packet packet;

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
				g.handleEvent(event);
                window.close();
			}
			if(g.handleEvent(event)==-1)
			{
				window.close();
			}
        }

		Socket.receive(packet);
		g.receivePacket(packet);

        window.clear();
        
		g.draw(window);

        window.display();
    }

	return 0;
}