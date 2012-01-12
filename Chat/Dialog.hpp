/*
 * Dialog.h
 *
 *  Created on: 2011-12-25
 *      Author: Dimitri
 */

#ifndef DIALOG_H_
#define DIALOG_H_

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>

class Dialog {
private:
	static char* filename;
public:
	Dialog();
	virtual ~Dialog();

	static char* saveFileDialog(HWND parent) {
		OPENFILENAME ofn;
		char *szFileName = (char*) malloc(MAX_PATH);

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "Text Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = "jpg";

		if (!GetSaveFileName(&ofn)) {
			return NULL;
		}
		return szFileName;
	}
	static char* openFileDialog(HWND parent) {
		OPENFILENAME ofn;
		char * szFileName = (char*) malloc(MAX_PATH);

		ZeroMemory(&ofn,sizeof(ofn));

		ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
		ofn.hwndOwner = parent;
		ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = "txt";

		if (!GetOpenFileName(&ofn)) {
			return NULL;
		}

		return szFileName;
	}
	static HFONT fontDialog(HWND parent) {
		CHOOSEFONT cf;
		LOGFONT lf;

		//Setting the CHOOSEFONT structure
		cf.lStructSize = sizeof(CHOOSEFONT);
		cf.hwndOwner = parent;
		cf.lpLogFont = &lf;
		cf.iPointSize = 0;
		cf.Flags = CF_SCREENFONTS;
		cf.rgbColors = RGB(0,0,0);
		cf.lCustData = 0L;
		cf.lpfnHook = (LPCFHOOKPROC) NULL;
		cf.lpTemplateName = (LPSTR) NULL;
		cf.hInstance = (HINSTANCE) NULL;
		cf.lpszStyle = (LPSTR) NULL;
		cf.nFontType = SCREEN_FONTTYPE;
		cf.nSizeMin = 0;
		cf.nSizeMax = 0;

		//Display the choose font dialog box.
		ChooseFont(&cf);

		//Create logical font based on user's selection
		return CreateFontIndirect(cf.lpLogFont);
	}
	static COLORREF colorDialog(HWND parent) {
		CHOOSECOLOR cc;
		static COLORREF acrCustClr[16];
		static DWORD rgbCurrent;

		// Initialize CHOOSECOLOR
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = parent;
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (!ChooseColor(&cc)) {
			return RGB(0,0,0);
		}
		return cc.rgbResult;
	}

	static void Message(char * message) {
		MessageBox(NULL, message, "Message Requested!", MB_OK | MB_ICONWARNING);
	}

	static void Debug(void * value) {
		char * buffer = (char*) value;
		MessageBox(NULL, buffer, "Message Requested!", MB_OK | MB_ICONWARNING);
	}
};

#endif /* DIALOG_H_ */
