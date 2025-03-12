#pragma once
#include <SFML/Graphics.hpp>
class Game
{
private:
	static const int BOARD_SIZE = 3 ;
	char board[BOARD_SIZE][BOARD_SIZE];
	int x;
public:
	Game();
	void Display_board();
	bool is_Win();
	void move(int player,bool &war, sf::RenderWindow& window);
	bool is_Legal(int row,int col);
	bool is_Draw();
	void draw(sf::RenderWindow& window);
};