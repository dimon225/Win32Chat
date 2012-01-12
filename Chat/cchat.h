/*
 * cchat.h
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#ifndef CCHAT_H_
#define CCHAT_H_

#include <windows.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <commctrl.h>

//my includes
#include "Dialog.hpp"
#include "Properties.h"
#include "Control.h"
#include "Server.h"
#include "Client.h"
#include "FileTransfer.h"
#include "Form.h"

char title[50] = "CChat Project.";

#define BUTTON_CANCEL 1001
#define BUTTON_SEND 1002
#define MENU_CONNECT_TO_SERVER 1003
#define MENU_EXIT 1004
#define MENU_SAVE 1005
#define MENU_SAVEAS 1006
#define LABEL_SEND 1007
#define TEXT_SEND 1008
#define MENU_CONNECT_AS_SERVER 1009
#define CONNECT_DIALOG 1010
#define FORM_MAIN_ID 1011
#define DIALOG_ID 1012
#define COLOR_DIALOG 1013
#define FONT_DIALOG 1014
#define HELP_MENU 1015
#define ABOUT_MENU 1016
#define DOWNLOAD_BUTTON 1017
#define UPLOAD_BUTTON 1018
#define BUTTON_D_CANCEL 1019
#define BUTTON_OK 1020
#define MENU_LOAD 1021
#define MENU_LOAD_FROM 1022

enum APP_TYPE {
	CLIENT = 0, SERVER = 1
};

#ifdef WIN32
#define OSTYPE "Windows"
#else
#define OSTYPE "Linux"
#endif

UINT formStyle = WS_SYSMENU | WS_CAPTION;
UINT txtChatStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE;
UINT btnSendStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK txtSendProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK dialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK creditsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK helpProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//forms
void FormOnLoad(HINSTANCE hInstance);
void DialogOnLoad(HINSTANCE hInstance);
void CreditsOnLoad(HINSTANCE hInstance);
void HelpOnLoad(HINSTANCE hInstance);

//initailzes the applicaton
void CreateChatMenu(HWND hwnd);
void drawInitialGraphics(HDC hdc, PAINTSTRUCT ps, LOGFONT font, HFONT hfont,
		HWND handle, char * value);
void cleanUp();
bool OnCreate(const HWND hwnd);
inline void error(const char* message);
void showKeyPressed(LPARAM lParam);
char* getKey(LPARAM lParam);
void sendChatMessage();
void StartTransferThread();
void start_listening();
void connect_to_server();

//thread functions
void * Thread_Server(void * arg);
void * Thread_Client(void * arg);
void * StartServer(void * args);
void * Thread_Listen(void * args);
void * Thread_Connect(void * args);

void startClientThread();
void startServerThread();

#endif /* CCHAT_H_ */
