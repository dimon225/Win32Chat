/*
 * Button.h
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <windows.h>

class Control {
private:
	WNDPROC wndProc;
	HINSTANCE app;
	HWND handle;
	LOGFONT logFont;
	HFONT font;
	HDC handleContext;
	RECT rect;
	HWND parent;
	int controlID;
	char* text;
	int x;
	int y;
	int width;
	int height;
	char* type;
	int style;
public:
	~Control();
	Control(int x, int y, int width, int height, HWND parent,
			HINSTANCE hInstance, char* text, char* type, int style,
			int controlId);
	HWND getHandle();
	HDC getHandleContext();
	LOGFONT getLogFont();
	HFONT getFont();
	HBRUSH getSolidBrush(COLORREF color);
	RECT getBounds();
	WNDPROC getWndProc();
	void setText(char* text);
	void clearText();
	char* getText();
	void setId(int controlID);
	int getId();
	void setFont(int italic, int size);
	void setFont(HFONT font);
	void setFont(LOGFONT logFont);
	void setForeGround(COLORREF color);
	void setBackground(COLORREF color);
	void setBounds(RECT rect);
	void setBounds(int x, int y, int width, int height);
	void invalidate();
	void setWndProc(WNDPROC wndProc);
	void sendMessage(int messageType, WPARAM messageValue);
	char * getTitle();
};

#endif /* BUTTON_H_ */
