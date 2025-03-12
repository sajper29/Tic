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


int __cdecl main(int argc, char** argv)
{
 
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_ADDR, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

	sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Tic Tac Toe Gracz 2",sf::Style::Default);
    window.setPosition({ 0,0 });

	Game game;
	int player1=1;
	int player2 = 2;
	int count=0;
    int currentplayer = 1;
	bool war = true;
	while (window.isOpen())
	{
        if (currentplayer == 1)
        {
            if (count == 0)
            {
                window.clear();
                game.draw(window);
                window.display();
            }
            game.move(player1, war, window, ConnectSocket);
            currentplayer = 2;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (currentplayer == 2)
            {
                game.move(player2, war, window, ConnectSocket);
                currentplayer = 1;

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
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}