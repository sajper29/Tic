#include <SFML/Graphics.hpp>
#include<iostream>
#include "Game.h";
using namespace std;
int main()
{
	sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Tic Tac Toe");
	Game game;
	int player1=1;
	int player2 = 2;
	int count=0;
	bool war = true;
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		window.clear();
		game.draw(window);
		window.display();
	}
	while (war==true)
	{
		game.Display_board();
		if (count % 2 == 0)
		{
			game.move(player1,war);
		}
		if (count % 2 == 1)
		{
			game.move(player2,war);
		}
		count++;
	}
	game.Display_board();
	
}