#pragma once
#include <SFML/Graphics.hpp>
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
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
	void move(int player,bool &war, sf::RenderWindow& window,SOCKET ClientSocket);
	bool is_Legal(int row,int col);
	bool is_Draw();
	void draw(sf::RenderWindow& window);
	void sendMove(SOCKET ClientSocket, int row, int col);
	void receiveMove(SOCKET ClientSocket, int& row, int& col);
};