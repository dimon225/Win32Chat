/*
 * Server.cpp
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include <vector>
#include <string.h>
#include <exception>
#include <iostream>
#include <windows.h>
#include <pthread.h>

using namespace std;

Server::~Server() {
	std::vector<int>::iterator iter;
	for (iter = clients.begin(); iter != clients.end(); iter++) {
		close(sock);
	}
	close(sock);
}

Server::Server() {
	this->sock = -1;
	this->client = -1;
	memset(&ip, 0, sizeof(ip));
}

Server::Server(int port, char* host, HWND handler) {
	this->sock = -1;
	this->client = -1;
	memset(&ip, 0, sizeof(ip));
	this->initialize(port, host, handler);
}

void Server::setHandler(HWND handler) {
	this->handler = handler;
}

void Server::initialize(int port, char* host, HWND handler) {
	this->port = port;
	this->host = host;
	this->handler = handler;
	ip.sin_family = AF_INET;
	ip.sin_port = htons(port);
	ip.sin_addr.s_addr = inet_addr(host);
}

void Server::setHost(char* host) {
	this->host = host;
}

void Server::setPort(int port) {
	this->port = port;
}

char* Server::getHost() {
	return this->host;
}

int Server::getPort() {
	return this->port;
}

std::vector<int> Server::getClients() {
	return clients;
}

void Server::sendMessage(char* buffer) {
	send(this->client, buffer, sizeof(char) * strlen(buffer), 0);
}

void Server::receiveMessage() {
	char buffer[4096];
	char temp[248];
	GetWindowText(this->handler, buffer, sizeof(buffer));
	int bytesRecieved = recv(this->client, temp, sizeof(temp), 0);
	temp[bytesRecieved] = '\0';
	strcat(buffer, "\n");
	strcat(buffer, "Client: ");
	strcat(buffer, temp);
	SendMessage(this->handler, WM_SETTEXT, 0, (LPARAM) temp);
}

int Server::getClient() {
	return this->client;
}

void Server::kill(int sock) {
	std::vector<int>::iterator iter;
	for (iter = clients.begin(); iter != clients.end(); iter++) {
		if (sock == *iter) {
			clients.erase(iter);
			close(sock);
		}
	}
}

void Server::startServer() {
	try {

		this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (this->sock == -1) {
			throw new std::exception();
		}

		bind(this->sock, (struct sockaddr *) &this->ip, sizeof(this->ip));

		listen(this->sock, 3);

		this->client = accept(this->sock, NULL, NULL);

	} catch (int& exception) {
		cout << "value of socket is " << this->sock << endl;
	}
}

void Server::stopServer() {
	if (client != -1) {
		shutdown(client, SHUT_RDWR);
		close(client);
	}
	if (sock != -1) {
		close(sock);
	}
}
