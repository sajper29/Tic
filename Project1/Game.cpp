#include<iostream>
#include "Game.h"
#include <SFML/Graphics.hpp>
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")

Game::Game()
{
	char temp[BOARD_SIZE][BOARD_SIZE] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	x = 9;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			board[i][j] = temp[i][j];
		}
	}
}
void Game::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::White);
	
	float width = window.getSize().x;
	float height = window.getSize().y;
	float cellSize = width / 3.0f;
	sf::RectangleShape line(sf::Vector2f(width, 5));

	line.setFillColor(sf::Color::Black);
	for (int i = 0; i < 3; ++i) {
		line.setPosition({ 0, i * cellSize });
		window.draw(line);
	}
	line.setPosition({ 0,595 });
	window.draw(line);
	line.setSize(sf::Vector2f(height, 5));
	line.setRotation(sf::degrees(90));

	for (int i = 1; i < 3; ++i) {
		line.setPosition({ i * cellSize, 0 });
		window.draw(line);
	}
	line.setPosition({ 5,0 });
	window.draw(line);
	line.setPosition({ 600,0 });
	window.draw(line);
	sf::Font font("ARIAL.ttf");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == 'X' || board[i][j] == 'O') {
				sf::Text text(font);
				text.setString((1, board[i][j]));
				text.setCharacterSize(50);
				text.setFillColor(sf::Color::Black);
				text.setPosition({ j * cellSize + cellSize / 3, i * cellSize + cellSize / 4 });
				window.draw(text);
			}
		}
	}
}

void Game::Display_board()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			std::cout << this->board[i][j]<<" ";
		}
		std::cout << std::endl;
	}
}
bool Game::is_Win()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (board[i][0] == board[i][1] && board[i][1]==board[i][2])
		{
			return true;
		}
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			return true;
		}
		if (board[0][0] == board[1][1] && board[1][1] == board[2][2] || board[0][2] == board[1][1] && board[1][1] == board[2][0])
		{
			return true;
		}
	}
	return false;
}
bool Game::is_Draw()
{

	if (is_Win() == true)
	{
		return false;
	}
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] != 'X' && board[i][j] != 'O')
			{
				return false;
			}
		}
	}
	return true;
	
}
void Game::move(int player,bool &war, sf::RenderWindow& window,SOCKET ClientSocket)
{
	int move,row,col,x,y;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		float cellSize = window.getSize().x / 3.0f;
		
		if (player == 1)
		{
			row = mousePos.y / cellSize;
			col = mousePos.x / cellSize;
		}
		else
		{
			receiveMove(ClientSocket, row, col);
		}
		bool czy = false;
		while (czy == false)
		{
			if (this->is_Legal(row, col) == true)
			{
				if (player == 1)
				{
					board[row][col] = 'O';
					sendMove(ClientSocket, row, col);
				}
				else
				{
					board[row][col] = 'X';
				}
				czy = true;
			}
			else
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					mousePos = sf::Mouse::getPosition(window);
					row = mousePos.y / cellSize;
					col = mousePos.x / cellSize;
				}
			}
			
		}


		if ((is_Draw() == true) || (is_Win() == true))
		{
			war = false;
			std::cout << "Koniec" << std::endl;
			if (is_Win() == true)
			{
				std::cout << "Wygrywa gracz " << player << std::endl;
			}
			if (is_Draw() == true)
			{
				std::cout << "Remis" << std::endl;
			}

		}
		
}
void Game::sendMove(SOCKET clientSocket, int row, int col) {
	char move[2] = { (char)(row + '0'), (char)(col + '0') };
	send(clientSocket, move, sizeof(move), 0);
}
void Game::receiveMove(SOCKET clientSocket, int& row, int& col) {
	char move[2];
	recv(clientSocket, move, sizeof(move), 0);
	row = move[0] - '0';
	col = move[1] - '0';
}
bool Game::is_Legal(int row, int col)
{
	if ((row >= 0 && row < 3) && (col >= 0 && col < 3 ))
	{
		if (board[row][col] != 'X' && board[row][col] != 'O')
		{
			return true;
		}
			
	}
	else {
		return false;
	}
}