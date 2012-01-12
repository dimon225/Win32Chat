//============================================================================
// Name        : CChat.cpp
// Author      : Dimitri Pankov
// Version     :
// Copyright   : @Dimitri Inc.
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "cchat.h"

//main application instance
HINSTANCE instance;

//client & server type pointers
Server *server;
Client *client;
FileTransfer *transfer;
Properties * properties;

Form * main_form, *dialog_form, *credit_form, *help_form;
RECTANGLE * rect;

//main window controls
Control *txtChat, *btnSend, *btnExit, *txtSend, *lblSend, *btnDownload,
		*btnUpload, *progress, *lblProgress;

//dialog controls
Control * txtPort, *txtHost, *lblPort, *lblHost, *btnOk, *btnCancel;

//windows types declarations
HMENU fileMenu, fileSubMenu, fontSubMenu, helpSubMenu;
HFONT hfont;
HDC hdc;
LOGFONT font;
PAINTSTRUCT ps;

static int type = 1;

static int port = 7777;
static char * host = "127.0.0.1";

char * filename = "properties.xml";

pthread_t server_thread, client_thread, transfer_thread, listen_thread,
		connect_thread;

//desktop properties
int desktopwidth = GetSystemMetrics(SM_CXSCREEN);
int desktopheight = GetSystemMetrics(SM_CYSCREEN);

HFONT __font = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, FALSE, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0,
		"Times New Roman");

LRESULT CALLBACK txtSendProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {

	switch (msg) {
	case WM_RBUTTONDBLCLK:
		error("you have double clicked txtChat");
		break;
	case WM_KEYDOWN:
		char *key = getKey(lParam);
		if (strcmp("Enter", key) == 0 || strcmp("Num Enter", key) == 0) {
			sendChatMessage();
		}
		SetFocus(txtSend->getHandle());
		free(key);
		break;
	}

	return CallWindowProc(txtSend->getWndProc(), hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK helpProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CLOSE:
		help_form->close();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BUTTON_OK:
			help_form->close();
			break;
		}
		break;
	case WM_PAINT:
		char buffer[50] = "";
		size_t buf_len = sizeof(buffer);
		hdc = BeginPaint(hwnd, &ps);
		font.lfStrikeOut = 0;
		font.lfUnderline = FALSE;
		font.lfHeight = 32;
		font.lfEscapement = 0;
		font.lfItalic = TRUE;
		hfont = CreateFontIndirect(&font);
		SelectObject(hdc, hfont);
		SetBkColor(hdc, RGB(205,127,50));
		SetTextColor(hdc, RGB(165,42,42));
		TextOut(hdc, 15, 1, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Author: Dimitri Pankov",
				strlen("Author: Dimitri Pankov"));
		TextOut(hdc, 15, 35, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Date: January 1st, 2012",
				strlen("Date: January 1st, 2012"));
		TextOut(hdc, 15, 70, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Contact: http://domnodes.com",
				strlen("Contact: http://domnodes.com"));
		TextOut(hdc, 15, 105, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "CopyRight @ DP I.T. inc.",
				strlen("CopyRight @ DP I.T. inc."));
		TextOut(hdc, 15, 140, buffer, buf_len);
		EndPaint(hwnd, &ps);
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK creditsProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {

	switch (msg) {
	case WM_CLOSE:
		credit_form->close();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BUTTON_OK:
			credit_form->close();
			break;
		}
		break;
	case WM_PAINT: {
		char buffer[50] = "";
		size_t buf_len = sizeof(buffer);
		hdc = BeginPaint(hwnd, &ps);
		font.lfStrikeOut = 0;
		font.lfUnderline = FALSE;
		font.lfHeight = 32;
		font.lfEscapement = 0;
		font.lfItalic = TRUE;
		hfont = CreateFontIndirect(&font);
		SelectObject(hdc, hfont);
		SetBkColor(hdc, RGB(205,127,50));
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 15, 1, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Author: Dimitri Pankov",
				strlen("Author: Dimitri Pankov"));
		TextOut(hdc, 15, 35, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Date: January 1st, 2012",
				strlen("Date: January 1st, 2012"));
		TextOut(hdc, 15, 70, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "Build: December 31, 2011",
				strlen("Build: December 31, 2011"));
		TextOut(hdc, 15, 105, buffer, buf_len);
		buffer[0] = '\0';
		memmove(buffer, "CopyRight @ DP I.T. inc.",
				strlen("CopyRight @ DP I.T. inc."));
		TextOut(hdc, 15, 140, buffer, buf_len);
		EndPaint(hwnd, &ps);
	}
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK dialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_PAINT:
		drawInitialGraphics(hdc, ps, font, hfont, dialog_form->getHandle(),
				"Network Preferences");
		break;
	case WM_SHOWWINDOW:
		txtPort->setFont(__font);
		txtHost->setFont(__font);
		lblPort->setFont(__font);
		lblHost->setFont(__font);
		btnOk->setFont(__font);
		btnCancel->setFont(__font);
		break;
	case WM_CTLCOLORSTATIC:
		lblPort->setBackground(TRANSPARENT);
		lblHost->setBackground(TRANSPARENT);
		return (INT_PTR) CreateSolidBrush(RGB(120,120,120));
		break;
	case WM_CLOSE:
		dialog_form->close();
		break;
	case WM_DESTROY:
		dialog_form->close();
		break;
	case WM_COMMAND: {
		WORD command = LOWORD(wParam);
		switch (command) {
		case BUTTON_D_CANCEL:
			dialog_form->close();
			break;
		case BUTTON_OK:
			port = atoi(txtPort->getText());
			host = txtHost->getText();
			properties->clear();
			properties->addProperty("port", txtPort->getText());
			properties->addProperty("host", host);
			dialog_form->close();
			break;
		}
	}
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CREATE:
		return OnCreate(hwnd);
		break;
	case WM_SHOWWINDOW: {
		lblSend->setFont(__font);
		lblProgress->setFont(__font);
		txtSend->setFont(__font);
		btnDownload->setFont(__font);
		btnExit->setFont(__font);
		btnSend->setFont(__font);
		btnUpload->setFont(__font);
		lblProgress->setBackground(RGB(255,0,0));
		lblSend->setBackground(RGB(255,0,0));
		lblProgress->setForeGround(RGB(255,0,0));
		lblSend->setForeGround(RGB(255,0,0));
		properties = new Properties();
		properties->addProperty("port", "7777");
		properties->addProperty("host", host);
	}
		break;
	case WM_CTLCOLORSTATIC:
		return (INT_PTR) lblSend->getSolidBrush(RGB(100,149,237));
	case WM_CLOSE:
		PostQuitMessage(WM_QUIT);
		break;
	case WM_DESTROY:
		cleanUp();
		dialog_form->close();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case DOWNLOAD_BUTTON:
			Dialog::Message("Does nothing for now.");
			break;
		case UPLOAD_BUTTON: {
			EnableWindow(btnUpload->getHandle(), FALSE);
			char* filename = Dialog::openFileDialog(main_form->getHandle());

			if (filename != NULL) {
				transfer->upload(filename, host, port);
			}

			EnableWindow(btnUpload->getHandle(), TRUE);
		}
			break;
		case CONNECT_DIALOG:
			DialogOnLoad(instance);
			dialog_form->show();
			break;
		case BUTTON_CANCEL:
			PostQuitMessage(WM_QUIT);
			break;
		case COLOR_DIALOG:
			COLORREF color = Dialog::colorDialog(main_form->getHandle());
			SetBkColor(txtChat->getHandleContext(), color);
			break;
		case FONT_DIALOG: {
			HFONT font = Dialog::fontDialog(main_form->getHandle());
			if (font != NULL) {
				txtChat->setFont(font);
			}
			DeleteObject(font);
		}
			break;
		case BUTTON_SEND:
			sendChatMessage();
			break;
		case MENU_EXIT:
			PostQuitMessage(18);
			break;
		case MENU_CONNECT_TO_SERVER:
			type = 0;
			main_form->setText("Client");
			connect_to_server();
			break;
		case MENU_CONNECT_AS_SERVER:
			type = 1;
			txtChat->setText("Waiting for clients to connect...");
			main_form->setText("Server");
			start_listening();
			break;
		case ABOUT_MENU:
			CreditsOnLoad(instance);
			credit_form->show();
			break;
		case HELP_MENU:
			HelpOnLoad(instance);
			help_form->show();
			break;
		case MENU_SAVE:
			properties->saveProperties(filename);
			break;
		case MENU_SAVEAS:
			properties->saveProperties(
					Dialog::saveFileDialog(main_form->getHandle()));
			break;
		case MENU_LOAD:
			properties->loadProperties(filename);
			port = atoi(properties->getProperty("port"));
			host = properties->getProperty("host");
			break;
		case MENU_LOAD_FROM:
			properties->loadProperties(
					Dialog::openFileDialog(main_form->getHandle()));
			port = atoi(properties->getProperty("port"));
			host = properties->getProperty("host");
			break;
		}
		break;
	case WM_PAINT:
		drawInitialGraphics(hdc, ps, font, hfont, main_form->getHandle(),
				title);
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

/*main function of the application creates main form and starts pumping message to controls both hiword and loword*/
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
		LPSTR cmdLineArgs, int style) {

	instance = hInstance;

	//load main form
	FormOnLoad(hInstance);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return static_cast<int>(msg.wParam);
}

void HelpOnLoad(HINSTANCE hInstance) {

	//initialize and show main form
	rect = new RECTANGLE;

	memset(rect, 0, sizeof(RECTANGLE));

	rect->x = desktopwidth / 4;
	rect->y = desktopheight / 4;
	rect->width = 550;
	rect->height = 230;

	help_form = new Form(hInstance, helpProc, main_form->getHandle());
	help_form->setClassName("help_dialog");
	help_form->setAppInstance(instance);
	help_form->initialize("Help Dialog", (DWORD) formStyle, rect,
			RGB(205,127,50));

	//button ok
	btnOk = new Control(425, 150, 100, 40, help_form->getHandle(), hInstance,
			"OK", "BUTTON", btnSendStyle, BUTTON_OK);
}

void CreditsOnLoad(HINSTANCE hInstance) {

	//initialize and show main form
	rect = new RECTANGLE;

	memset(rect, 0, sizeof(RECTANGLE));

	rect->x = desktopwidth / 4;
	rect->y = desktopheight / 4;
	rect->width = 550;
	rect->height = 230;

	credit_form = new Form(hInstance, creditsProc, main_form->getHandle());
	credit_form->setClassName("credit_form");
	credit_form->setAppInstance(instance);
	credit_form->initialize("Credits", (DWORD) formStyle, rect,
			RGB(205,127,50));

	//button ok
	btnOk = new Control(425, 150, 100, 40, credit_form->getHandle(), hInstance,
			"OK", "BUTTON", btnSendStyle, BUTTON_OK);
}

void DialogOnLoad(HINSTANCE hInstance) {

	//initialize and show main form
	rect = new RECTANGLE;

	memset(rect, 0, sizeof(RECTANGLE));

	rect->x = desktopwidth / 4;
	rect->y = desktopheight / 4;
	rect->width = 550;
	rect->height = 230;

	dialog_form = new Form(hInstance, dialogProc, main_form->getHandle());
	dialog_form->setClassName("dialog_form");
	dialog_form->setAppInstance(instance);
	dialog_form->initialize("Network Preferences", (DWORD) formStyle, rect,
			RGB(120,120,120));

	//label port
	lblPort = new Control(20, 55, 100, 30, dialog_form->getHandle(), hInstance,
			"Port: ", "STATIC", WS_CHILD | WS_VISIBLE | SS_LEFT
			, 0);

	lblPort->setWndProc(dialogProc);

	//txt port
	txtPort = new Control(135, 55, 390, 35, dialog_form->getHandle(), hInstance,
			"", "EDIT", WS_CHILD | WS_VISIBLE, 0);

	txtPort->setWndProc(dialogProc);

	//label host
	lblHost = new Control(20, 100, 100, 30, dialog_form->getHandle(), hInstance,
			"Server IP: ", "STATIC", WS_CHILD | WS_VISIBLE | SS_LEFT, 0);

	lblHost->setWndProc(dialogProc);

	//txt host
	txtHost = new Control(135, 100, 390, 35, dialog_form->getHandle(),
			hInstance, "", "EDIT", WS_CHILD | WS_VISIBLE, 0);

	txtHost->setWndProc(dialogProc);

	//button ok
	btnOk = new Control(265, 145, 130, 40, dialog_form->getHandle(), hInstance,
			"Apply", "BUTTON", btnSendStyle, BUTTON_OK);

	btnOk->setWndProc(dialogProc);

	//button cancel
	btnCancel = new Control(400, 145, 130, 40, dialog_form->getHandle(),
			hInstance, "Cancel", "BUTTON", btnSendStyle, BUTTON_D_CANCEL);

	btnCancel->setWndProc(dialogProc);
}

void FormOnLoad(HINSTANCE hInstance) {

	//initialize and show main form
	rect = new RECTANGLE;

	memset(rect, 0, sizeof(RECTANGLE));

	rect->x = desktopwidth / 4;
	rect->y = desktopheight / 4;
	rect->width = 700;
	rect->height = 620;

	main_form = new Form(hInstance, wndProc, NULL);
	main_form->setClassName("main_form");
	main_form->initialize("CCHAT", (DWORD) formStyle, rect, RGB(120,120,120));

	//text box multiline for chat
	txtChat = new Control(20, 65, 650, 400, main_form->getHandle(), hInstance,
			"", "EDIT", txtChatStyle, 0);

	txtChat->setWndProc(wndProc);

	//button send
	btnSend = new Control(420, 470, 130, 40, main_form->getHandle(), hInstance,
			"SEND", "BUTTON", btnSendStyle, BUTTON_SEND);

	btnSend->setWndProc(wndProc);

	//button exit
	btnExit = new Control(550, 470, 130, 40, main_form->getHandle(), hInstance,
			"CANCEL", "BUTTON", btnSendStyle, BUTTON_CANCEL);

	btnExit->setWndProc(wndProc);

	//text send
	txtSend = new Control(180, 475, 230, 35, main_form->getHandle(), hInstance,
			"", "EDIT", WS_CHILD | WS_VISIBLE, TEXT_SEND);

	//define the txt send procedure
	txtSend->setWndProc(
			(WNDPROC) SetWindowLong(txtSend->getHandle(), GWL_WNDPROC,
					(LONG) txtSendProc));

	//label send
	lblSend = new Control(50, 475, 100, 35, main_form->getHandle(), hInstance,
			"Send:", "STATIC", WS_CHILD | WS_VISIBLE | SS_LEFT
			, LABEL_SEND);

	lblSend->setWndProc(wndProc);

	//button download
	btnDownload = new Control(420, 515, 130, 40, main_form->getHandle(),
			hInstance, "Download", "BUTTON", btnSendStyle,
			(UINT) DOWNLOAD_BUTTON);

	btnDownload->setWndProc(wndProc);

	//button upload
	btnUpload = new Control(550, 515, 130, 40, main_form->getHandle(),
			hInstance, "Upload", "BUTTON", btnSendStyle, (UINT) UPLOAD_BUTTON);

	btnUpload->setWndProc(wndProc);

	//progress bar
	progress = new Control(150, 522, 250, 25, main_form->getHandle(), hInstance,
			"Upload", PROGRESS_CLASS, WS_CHILD | WS_VISIBLE
			, (UINT) 1);

	progress->setWndProc(wndProc);

	//label progress
	lblProgress = new Control(10, 515, 135, 35, main_form->getHandle(),
			hInstance, "Download", "STATIC", WS_CHILD | WS_VISIBLE
			, (UINT) UPLOAD_BUTTON);

	lblProgress->setWndProc(wndProc);

	StartTransferThread();

	main_form->show();
}

void StartTransferThread() {
	transfer = new FileTransfer();
	transfer->setControl(progress);
	pthread_create(&transfer_thread, NULL, &StartServer, NULL);
}

void * StartServer(void * args) {
	int sock;
	struct sockaddr_in ip;
	int client;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1) {
		perror("Error creating server socket");
	}

	memset(&ip, 0, sizeof(ip));
	ip.sin_family = AF_INET;
	ip.sin_port = htons(port);
	ip.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr*) &ip, sizeof(ip)) == -1) {
		perror("Error binding server to port in transfer thread");
		return NULL;
	}

	if (listen(sock, 10) == -1) {
		perror("Error listening for connections");
		return NULL;
	}

	while (TRUE) {
		client = accept(sock, NULL, NULL);
		if (client == -1) {
			break;
		}
		//accepted client start download
		transfer->download(client);
	}
	return NULL;
}

void connect_to_server() {
	if (pthread_create(&connect_thread, NULL, &Thread_Connect, NULL)) {
		perror("Error creating listen thread..");
	}
}

void * Thread_Connect(void * args) {
	client = new Client(port, host, txtChat->getHandle());
	client->connectToServer();
	txtChat->setText("Welcome server is online...");
	startClientThread();
	return NULL;
}

void start_listening() {
	if (pthread_create(&listen_thread, NULL, &Thread_Listen, NULL)) {
		perror("Error creating listen thread..");
	}
}

void * Thread_Listen(void * args) {
	server = new Server(port, host, txtChat->getHandle());
	server->startServer();
	startServerThread();
	txtChat->setText("Accepted a connection...");
	return NULL;
}

void * Thread_Server(void * arg) {
	register int sock = server->getClient();
	while (TRUE) {
		char *buffer;
		char temp[248];
		buffer = txtChat->getText();
		int bytesRecieved = recv(sock, temp, sizeof(temp), 0);
		temp[bytesRecieved] = '\0';

		if (strcmp(temp, "Q") == 0 || strcmp(temp, "q") == 0
				|| strcmp(temp, "") == 0) {
			strcat(buffer, "\r\n");
			strcat(buffer, "Client has disconnected.");
			strcat(buffer, temp);
			txtChat->setText(buffer);
			break;
		}

		strcat(buffer, "\r\n");
		strcat(buffer, "Client: ");
		strcat(buffer, temp);
		txtChat->setText(buffer);
		free(buffer);
	}
	return NULL;
}

void * Thread_Client(void * arg) {
	register int sock = client->getSocket();
	while (TRUE) {
		char *buffer;
		char temp[248];
		buffer = txtChat->getText();
		int bytesRecieved = recv(sock, temp, sizeof(temp), 0);
		temp[bytesRecieved] = '\0';

		if (strcmp(temp, "Q") == 0 || strcmp(temp, "q") == 0
				|| strcmp(temp, "") == 0) {
			strcat(buffer, "\r\n");
			strcat(buffer, "Server has shutdown.");
			strcat(buffer, temp);
			txtChat->setText(buffer);
			break;
		}

		strcat(buffer, "\r\n");
		strcat(buffer, "Server: ");
		strcat(buffer, temp);
		txtChat->setText(buffer);
		free(buffer);
	}
	return NULL;
}

void startClientThread() {
	if (pthread_create(&client_thread, NULL, &Thread_Client, NULL)) {
		perror("Error creating server thread.");
	}
}

void startServerThread() {
	if (pthread_create(&server_thread, NULL, &Thread_Server, NULL)) {
		perror("Error creating server thread.");
	}
}

void CreateChatMenu(HWND hwnd) {
	fileMenu = CreateMenu();
	fileSubMenu = CreatePopupMenu();

	AppendMenuA(fileSubMenu, MF_STRING, CONNECT_DIALOG, "N&etwork Preferences");
	AppendMenuA(fileSubMenu, MF_STRING, MENU_CONNECT_TO_SERVER,
			"C&onnect To Server");

	AppendMenuA(fileSubMenu, MF_STRING, MENU_CONNECT_AS_SERVER,
			"C&onnect As Server");

	AppendMenuA(fileSubMenu, MF_STRING, MENU_SAVE, "S&ave");
	AppendMenuA(fileSubMenu, MF_STRING, MENU_SAVEAS, "S&ave As");
	AppendMenuA(fileSubMenu, MF_STRING, MENU_LOAD, "Load");
	AppendMenuA(fileSubMenu, MF_STRING, MENU_LOAD_FROM, "Load from file");
	AppendMenuA(fileSubMenu, MF_STRING, MENU_EXIT, "E&xit");
	AppendMenuA(fileMenu, MF_STRING | MF_POPUP, (UINT) fileSubMenu , "&File");

	fontSubMenu = CreatePopupMenu();
	AppendMenuA(fontSubMenu, MF_STRING, COLOR_DIALOG, "C&olor");
	AppendMenuA(fontSubMenu, MF_STRING, FONT_DIALOG, "F&ont");

	AppendMenuA(fileMenu, MF_STRING | MF_POPUP, (UINT) fontSubMenu , "&Editor");

	helpSubMenu = CreatePopupMenu();
	AppendMenuA(helpSubMenu, MF_STRING, HELP_MENU, "Help");
	AppendMenuA(helpSubMenu, MF_STRING, ABOUT_MENU, "Credits");

	AppendMenuA(fileMenu, MF_STRING | MF_POPUP, (UINT) helpSubMenu , "About");

	SetMenu(hwnd, fileMenu);
}

void drawInitialGraphics(HDC hdc, PAINTSTRUCT ps, LOGFONT font, HFONT hfont,
		HWND handle, char * value) {
	hdc = BeginPaint(handle, &ps);
	font.lfStrikeOut = 0;
	font.lfUnderline = TRUE;
	font.lfHeight = 42;
	font.lfEscapement = 0;
	font.lfItalic = TRUE;
	hfont = CreateFontIndirect(&font);
	SelectObject(hdc, hfont);
	SetBkColor(hdc, RGB(0,0,255));
	SetTextColor(hdc, RGB(255, 25, 5));
	TextOut(hdc, 20, 10, value, strlen(value) + 1);
	EndPaint(handle, &ps);
}

bool OnCreate(const HWND hwnd) {
	CreateChatMenu(hwnd);
	return true;
}

void showKeyPressed(LPARAM lParam) {
	char key[20] = "";
	GetKeyNameTextA(lParam, key, sizeof(key));
	error(key);
}

char* getKey(LPARAM lParam) {
	char *key = (char*) malloc(sizeof(char) * 20);
	GetKeyNameTextA(lParam, key, sizeof(char) * 20);
	return key;
}

void sendChatMessage() {
	switch (type) {
	case SERVER:
		if (server != NULL) {
			server->sendMessage(txtSend->getText());
		}
		break;
	case CLIENT:
		if (client != NULL) {
			client->sendMessage(txtSend->getText());
		}
		break;
	}
	txtSend->clearText();
}

inline void error(const char* message) {
	MessageBox(main_form->getHandle(), message, "Error Message",
			MB_OK | MB_ICONWARNING);
}

void cleanUp() {
	DeleteObject(hfont);
	DeleteObject(hdc);
	DeleteObject(fileMenu);
	DeleteObject(fileSubMenu);
	DeleteObject(fontSubMenu);
	DeleteObject(helpSubMenu);
	delete server;
	delete client;
	delete server_thread;
	delete client_thread;
	delete main_form;
	delete txtChat;
	delete lblSend;
	delete btnExit;
	delete txtSend;
	delete transfer;
	delete rect;
	delete btnOk;
	delete btnCancel;
	delete txtPort;
	delete lblPort;
	delete txtHost;
	delete lblHost;
	delete dialog_form;
	delete credit_form;
	delete help_form;
	delete properties;
}
