/*
 * Server.h
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <windows.h>
#include <vector>
#include <arpa/inet.h>
#include <pthread.h>

class Server {
private:
	char* host;
	int sock;
	int client;
	int port;
	struct sockaddr_in ip;
	std::vector<int> clients;
	HWND handler;
public:
	~Server();
	Server();
	Server(int port, char* host, HWND handler);
	void setHandler(HWND handler);
	void startServer();
	void stopServer();
	void kill(int sock);
	std::vector<int> getClients();
	void initialize(int port, char* host, HWND handler);
	void setHost(char* host);
	void setPort(int port);
	char* getHost();
	int getPort();
	void sendMessage(char* buffer);
	void receiveMessage();
	int getClient();
};

#endif /* SERVER_H_ */
