/*
 * Client.cpp
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
#include "Client.h"
#include <vector>
#include <string.h>
#include <exception>
#include <iostream>
#include <windows.h>

using namespace std;

Client::~Client() {
	close(sock);
}

Client::Client() {
	this->sock = -1;
	this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&ip, 0, sizeof(ip));
}

Client::Client(int port, char* host, HWND handler) {
	this->sock = -1;
	this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->sock == -1) {
		perror("Cannot create socket");
	}
	memset(&ip, 0, sizeof(ip));
	this->initialize(port, host, handler);
}

void Client::setHandler(HWND handler) {
	this->handler = handler;
}

void Client::initialize(int port, char* host, HWND handler) {
	this->port = port;
	this->host = host;
	this->handler = handler;
	this->ip.sin_family = AF_INET;
	this->ip.sin_port = htons(port);
	this->ip.sin_addr.s_addr = inet_addr(host);
}

void Client::setHost(char* host) {
	this->host = host;
}

void Client::setPort(int port) {
	this->port = port;
}

char* Client::getHost() {
	return this->host;
}

int Client::getPort() {
	return this->port;
}

void Client::sendMessage(char* buffer) {
	send(this->sock, buffer, sizeof(char) * strlen(buffer), 0);
}

void Client::connectToServer() {
	connect(this->sock, (struct sockaddr*) &this->ip, sizeof(this->ip));
}

void Client::receiveMessage() {
	char buffer[4096];
	char temp[248];
	GetWindowText(this->handler, buffer, sizeof(buffer));
	int bytesRecieved = recv(sock, temp, sizeof(temp), 0);
	temp[bytesRecieved] = '\0';
	strcat(buffer, "\n");
	strcat(buffer, "Server: ");
	strcat(buffer, temp);
	SendMessage(this->handler, WM_SETTEXT, 0, (LPARAM) buffer);
}

int Client::getSocket() {
	return this->sock;
}

void Client::closeClient() {
	if (sock != -1) {
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
}

