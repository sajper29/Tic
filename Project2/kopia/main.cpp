#include <SFML/Graphics.hpp>
#include<iostream>
#include "Game.h";
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_ADDR "127.0.0.1"


int  main()
{
 
	sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Tic Tac Toe");
	Game game;
	int player1=1;
	int player2 = 2;
	int count=0;
	bool war = true;
	while (window.isOpen())
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (count % 2 == 0)
			{
				game.move(player1, war, window);

			}
			if (count % 2 == 1)
			{
				game.move(player2, war, window);

			}
		}
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		
		
		window.clear();
		game.draw(window);
		window.display();
		count++;
		if (war == false)
		{
			window.close();
		}
	}
	
}