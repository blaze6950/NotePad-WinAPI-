#pragma once
#include "header.h"

class NotePadDlg
{
public:
	NotePadDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static NotePadDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
	void Cls_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
	HWND hDialog, hStatus, hEdit;
	BOOL bShowStatusBar, bIsOpenF, SAVE;
	HMENU hMenuRU, hMenuEN;

	TCHAR FullPat[MAX_PATH];
	void openFile(HWND hwnd);
	void saveFile(HWND hwnd, int i);
};
