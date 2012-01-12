/*
 * Form.h
 *
 *  Created on: 2011-12-31
 *      Author: Dimitri
 */

#ifndef FORM_H_
#define FORM_H_

#include <windows.h>
#include <vector>
#include "Control.h"
#include "Dialog.hpp"
#include <string.h>

//RECTANGLE

typedef struct {
	int x;
	int y;
	int width;
	int height;
} RECTANGLE;

using namespace std;

class Form {
private:
	char * title;
	char * w_szClassName;
	HINSTANCE hInstance;
	RECTANGLE * rect;
	HWND handle;
	HDC handleContext;
	WNDPROC wndProc;
	vector<Control> controls;
	DWORD style;
	WNDCLASSEX wex;
	HWND parent;
public:
	Form(HINSTANCE hInstance, WNDPROC wndProc, HWND parent);
	Form();
	virtual ~Form();
	void setAppInstance(HINSTANCE instance);
	void unregister();
	void initialize(char * title, DWORD style, RECTANGLE * rect,COLORREF color);
	void addControl(const Control& control);
	void removeControl(Control* control);
	void setWndProc(WNDPROC wndProc);
	void setClassName(char * w_szClassName);
	LPSTR getClassName();
	HWND getHandle();
	HDC getHandleContext();
	void setText(char * text);
	void show();
	void hide();
	void close();
	const RECT * getWindowRect();
};

#endif /* FORM_H_ */
