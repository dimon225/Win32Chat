/*
 * Button.cpp
 *
 *  Created on: 2011-12-24
 *      Author: Dimitri
 */

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include "Dialog.hpp"
#include "Control.h"

Control::Control(int x, int y, int width, int height, HWND parent,
		HINSTANCE hInstance, char* text, char* type, int style, int controlId) {

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->parent = parent;
	this->app = hInstance;
	this->text = text;
	this->type = type;
	this->style = style;
	this->controlID = controlId;
	this->rect.top = x;
	this->rect.left = y;
	this->rect.bottom = height;
	this->rect.right = width;

	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, type, text, style, x, y,
			width, height, parent, (HMENU) controlId, hInstance, NULL);

	this->handleContext = GetDC(this->handle);
}

void Control::setWndProc(WNDPROC wndProc) {
	this->wndProc = wndProc;
}

WNDPROC Control::getWndProc() {
	return this->wndProc;
}

void Control::invalidate() {
	InvalidateRect(this->handle, &this->rect, 1);
}

void Control::setText(char* text) {
	if (this->handle != NULL) {
		SendMessageA(this->handle, WM_SETTEXT, 0, (LPARAM) text);
	}
}

void Control::clearText() {
	if (this->handle != NULL) {
		SendMessageA(this->handle, WM_SETTEXT, 0, (LPARAM) "");
	}
}

char* Control::getText() {
	char* buffer = (char*) malloc(sizeof(char) * 1048);
	GetWindowTextA(this->handle, buffer, sizeof(char) * 1048);
	return buffer;
}

char * Control::getTitle() {
	return this->text;
}

void Control::sendMessage(int messageType, WPARAM messageValue) {
	SendMessageA(this->handle, messageType, (WPARAM) messageValue, 0);
}

void Control::setBackground(COLORREF color) {
	SetBkColor(this->handleContext, color);
}

void Control::setForeGround(COLORREF color) {
	SetTextColor(this->handleContext, RGB(255,0,0));
}

void Control::setFont(int italic, int size) {
	LOGFONT logFont;
	logFont.lfItalic = italic;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfHeight = size;
	this->logFont = logFont;
	SendMessageA(this->handle, WM_SETFONT,
			WPARAM(CreateFontIndirectA(&logFont)), TRUE);
}

void Control::setBounds(RECT rect) {
	this->rect = rect;
}

void Control::setBounds(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->rect.top = x;
	this->rect.left = y;
	this->rect.bottom = height;
	this->rect.right = width;
}

RECT Control::getBounds() {
	return this->rect;
}

HBRUSH Control::getSolidBrush(COLORREF color) {
	return CreateSolidBrush(color);
}

void Control::setFont(HFONT font) {
	this->font = font;
	SendMessageA(this->handle, WM_SETFONT, WPARAM(font), TRUE);
}

void Control::setFont(LOGFONT logFont) {
	this->logFont = logFont;
	SendMessageA(this->handle, WM_SETFONT,
			WPARAM(CreateFontIndirectA(&logFont)), TRUE);
}

LOGFONT Control::getLogFont() {
	return this->logFont;
}

HFONT Control::getFont() {
	return this->font;
}

HWND Control::getHandle() {
	return this->handle;
}

HDC Control::getHandleContext() {
	return this->handleContext;
}

void Control::setId(int controlID) {
	this->controlID = controlID;
}

int Control::getId() {
	return this->controlID;
}

Control::~Control() {
	DeleteObject(this->handle);
}

