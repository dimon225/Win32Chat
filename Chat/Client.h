/*
 * Client.cpp
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <windows.h>
#include <vector>
#include <arpa/inet.h>

class Client {
private:
	char* host;
	int sock;
	int port;
	struct sockaddr_in ip;
	std::vector<int> clients;
	HWND handler;
public:
	~Client();
	Client();
	Client(int port, char* host, HWND handler);
	void initialize(int port, char* host, HWND handler);
	void setHandler(HWND handler);
	void setHost(char* host);
	void setPort(int port);
	char* getHost();
	int getPort();
	void connectToServer();
	void sendMessage(char* buffer);
	void receiveMessage();
	int getSocket();
	void closeClient();
};

#endif
