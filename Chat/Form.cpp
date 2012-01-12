/*
 * Form.cpp
 *
 *  Created on: 2011-12-31
 *      Author: Dimitri
 */

#include "Form.h"

Form::Form() {

}

Form::~Form() {

}

Form::Form(HINSTANCE hInstance, WNDPROC wndProc, HWND parent) {
	this->hInstance = hInstance;
	this->wndProc = wndProc;
	this->parent = parent;

}

void Form::initialize(char * title, DWORD style, RECTANGLE * rect,
		COLORREF color) {
	this->title = title;
	this->style = style;
	this->rect = rect;

	this->unregister();

	ZeroMemory(&this->wex,sizeof(this->wex));

	this->wex.cbClsExtra = 0;
	this->wex.cbSize = sizeof(WNDCLASSEX);
	this->wex.cbWndExtra = 0;
	this->wex.hCursor = LoadCursor(NULL, IDC_ARROW);
	this->wex.hIcon = HICON(
			LoadImage(NULL, "Chat-rose.ico", IMAGE_ICON,
					GetSystemMetrics(SM_CXSMICON),
					GetSystemMetrics(SM_CXSMICON), LR_LOADFROMFILE));
	this->wex.hIconSm = HICON(
			LoadImage(NULL, "Chat-rose.ico", IMAGE_ICON,
					GetSystemMetrics(SM_CXSMICON),
					GetSystemMetrics(SM_CXSMICON), LR_LOADFROMFILE));
	this->wex.hInstance = this->hInstance;
	this->wex.hbrBackground = CreateSolidBrush(color);
	this->wex.lpfnWndProc = this->wndProc;
	this->wex.lpszClassName = this->w_szClassName;
	this->wex.lpszMenuName = 0;
	this->wex.style = 0;

	if (!RegisterClassEx(&this->wex)) {
		Dialog::Message("ClassEx not registered");
		return;
	}

	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, this->w_szClassName, title,
			style, rect->x, rect->y, rect->width, rect->height, parent,
			(HMENU) 0, hInstance, NULL);

	if (this->handle == NULL) {
		Dialog::Message("Handle is null");
		return;
	}

	this->handleContext = GetDC(this->handle);
}

void Form::setText(char * text) {
	if (this->handle != NULL) {
		SendMessageA(this->handle, WM_SETTEXT, 0, (LPARAM) text);
	}
}

void Form::addControl(const Control& control) {
	this->controls.push_back(control);
}

void Form::removeControl(Control* control) {
	vector<Control>::iterator it = controls.begin();
	for (; it != controls.end(); ++it) {
		if (strcmp(control->getTitle(), it->getText()) == 0) {
			this->controls.erase(it);
		}
	}
}

const RECT * Form::getWindowRect() {
	RECT * rectangle;
	rectangle->left = this->rect->x;
	rectangle->top = this->rect->y;
	rectangle->right = this->rect->width;
	rectangle->bottom = this->rect->height;
	return rectangle;
}

void Form::unregister() {
	UnregisterClassA(this->w_szClassName, this->hInstance);
}

void Form::setAppInstance(HINSTANCE instance) {
	this->hInstance = instance;
}

HWND Form::getHandle() {
	return this->handle;
}

LPSTR Form::getClassName() {
	return this->w_szClassName;
}

HDC Form::getHandleContext() {
	return this->handleContext;
}

void Form::setClassName(char * w_szClassName) {
	this->w_szClassName = w_szClassName;
}

void Form::show() {
	ShowWindow(this->handle, SW_SHOWNORMAL);
}

void Form::hide() {
	ShowWindow(this->handle, SW_HIDE);
}

void Form::close() {
	DestroyWindow(this->handle);
}

