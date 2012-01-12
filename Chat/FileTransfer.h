/*
 * FileTransfer.h
 *
 *  Created on: 2011-12-28
 *      Author: Dimitri
 */

#ifndef FILETRANSFER_H_
#define FILETRANSFER_H_

#include <pthread.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include "Dialog.hpp"
#include "Control.h"
#include <commctrl.h>

using namespace std;

class FileTransfer {
private:
	const int static BUFFER_SIZE = 4096;
	int fileMode;
	char* filename;
	int sock;
	int client;
	struct sockaddr_in ip;
	int port;
	pthread_t thread;
	Control * control;
public:
	FileTransfer();
	~FileTransfer();
	void download(int sock);
	void download();
	void upload(char* filename, char* ip, int port);
	void setFileName(char* filename);
	void * startServer(void * args);
	void setPort();
	int getPort();
	void setClient();
	int getClient();
	long getFileSize(const char* filename) const;
	char * getFileName(const char* filepath) const;
	void createDir(const char* name);
	void displayProgress(LPARAM value);
	void setControl(Control * control);
	Control * getTxtChat();

	template<class T>
	char * numberToString(T number);
};

#endif /* FILETRANSFER_H_ */
