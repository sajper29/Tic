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




int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;
    const int USERS = 4;
    int ILE = 0;
    int exit = 0;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    int ConnectionCounter = 0;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_ADDR, DEFAULT_PORT, &hints, &result);
    std::cout << iResult;
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    /*while (ILE < 2)
    {*/
        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
       /* else
        {
            printf("Client connected");
            ILE++;
            Connections[ConnectionCounter] = ClientSocket;
            ConnectionCounter++;
            if (ILE == 1)
            {
                std::string str = "Oczekuje na drugiego gracza";
                char* cstr = str.data();
                send(Connections[0], cstr, strlen(cstr), 0);
            }
        }*/
    
    // No longer need server set
    closesocket(ListenSocket);

   

	sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Tic Tac Toe Gracz 1", sf::Style::Default);
    window.setPosition({ 1000,0 });
	Game game;
	int player1=1;
	int player2 = 2;
    int currentplayer = 1;
	int count=0;
	bool war = true;
    while (window.isOpen())
    {
        if (currentplayer == 2)
        {
            game.move(player2, war, window, ClientSocket);
            currentplayer = 1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (currentplayer == 1)
            {
                game.move(player1, war, window, ClientSocket);

                currentplayer = 2;
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
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}