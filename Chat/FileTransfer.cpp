/*
 * FileTransfer.cpp
 *
 *  Created on: 2011-12-28
 *      Author: Dimitri
 */

#include "FileTransfer.h"

FileTransfer::~FileTransfer() {

}

FileTransfer::FileTransfer() {
	this->createDir("downloads/");
}

void FileTransfer::setFileName(char* filename) {
	this->filename = filename;
}

void FileTransfer::createDir(const char* name) {
	mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void FileTransfer::download(int client) {
	char buffer[BUFFER_SIZE];
	int bytesRead = 1;
	char filename[258];
	char filesize[50];
	long upper_limit = 0;

	int length = recv(client, filename, sizeof(filename), 0);

	filename[length] = '\0';

	send(client, "true", sizeof(char) * 4, 0);

	length = recv(client, filesize, sizeof(filesize), 0);

	filesize[length] = '\0';

	upper_limit = atol(filesize);

	char fullname[11] = "downloads/";

	fstream *file = new fstream(strcat(fullname, filename),
			ios::out | ios::binary);

	control->sendMessage(PBM_SETPOS,(WPARAM)10);

	do {
		bytesRead = recv(client, buffer, sizeof(buffer), 0);
		file->write(buffer, bytesRead);
		file->flush();
	} while (bytesRead > 0);

	file->close();
	close(client);
	delete file;
}

void FileTransfer::setControl(Control * control) {
	this->control = control;
}

Control * FileTransfer::getTxtChat() {
	return this->control;
}

void FileTransfer::upload(char* filepath, char* ip_addr, int port) {

	char buffer[BUFFER_SIZE];

	fstream *file = new fstream(filepath, ios::in | ios::binary);

	const char * filename = getFileName(filepath);

	const char * filesize = numberToString(getFileSize(filepath));

	this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->sock == -1) {
		perror("Error creating socket");
		return;
	}

	memset(&ip, 0, sizeof(ip));

	this->ip.sin_family = AF_INET;
	this->ip.sin_port = htons(port);
	this->ip.sin_addr.s_addr = inet_addr(ip_addr);

	if (connect(this->sock, (struct sockaddr*) &this->ip, sizeof(this->ip))
			== -1) {
		perror("connection not established to server");
		return;
	}

	//send filename
	send(this->sock, filename, strlen(filename), 0);

	//receive notification notification
	recv(this->sock, (void*) filename, sizeof(filename), 0);

	//send filesize
	send(this->sock, filesize, strlen(filesize), 0);

	do {
		file->read(buffer, BUFFER_SIZE);
		send(this->sock, buffer, file->gcount(), 0);
	} while (file->gcount() > 0);

	close(this->sock);
	delete file;
}

long FileTransfer::getFileSize(const char* filename) const {
	FILE *file;
	size_t size = 0;
	if ((file = fopen(filename, "rb")) != NULL) {
		if (fseek(file, 0, SEEK_END) == 0) {
			size = ftell(file);
			fclose(file);
		}
	}
	return size;
}

void FileTransfer::displayProgress(LPARAM value) {
	this->control->sendMessage(PBM_SETPOS, value);
}

char * FileTransfer::getFileName(const char* filepath) const {
	char * slash = strrchr(filepath, '\\');
	char * newFilename;
	if (slash == NULL) {
		return const_cast<char*>(filepath);
	}
	newFilename = strstr(filepath, slash);
	return ++newFilename;
}

template<class T>
char * FileTransfer::numberToString(T number) {
	char * buffer = (char*) malloc(25);
	sprintf(buffer, "%ld", number);
	return buffer;
}

void FileTransfer::download() {

}

void * FileTransfer::startServer(void * args) {
	while (true) {
		this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
		memset(&ip, 0, sizeof(ip));
		this->ip.sin_family = AF_INET;
		this->ip.sin_port = htons(this->port);
		this->ip.sin_addr.s_addr = INADDR_ANY;
		bind(this->sock, (struct sockaddr*) &this->ip, sizeof(ip));
		listen(this->sock, 10);
		this->client = accept(this->sock, NULL, NULL);

		if (client == -1) {
			break;
		}

		//accepted client start download
		this->download();
	}
	return NULL;
}

