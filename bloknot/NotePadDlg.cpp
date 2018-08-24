#include "NotePadDlg.h"

void saveFile(HWND hwnd);
void openFile(HWND hwnd);

NotePadDlg* NotePadDlg::ptr = NULL;

NotePadDlg::NotePadDlg(void)
{
	ptr = this;
	bShowStatusBar = TRUE;
	bIsOpenF = FALSE;
}

void NotePadDlg::Cls_OnClose(HWND hwnd)
{
	if (SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE)
	{
		if (MessageBox(hwnd, L"������ ��������� ���������?", L"����������", MB_YESNO) == 6)
		{
			saveFile(hwnd, 0);
			SendMessage(hEdit, WM_SETTEXT, 0, 0);
		}
		else{
			EndDialog(hwnd, 0);
		}
	}
	else{
		EndDialog(hwnd, 0);
	}
}

BOOL NotePadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	// ������� ���������� ���������� ����
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	// �������� ������ ���������
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDialog, WM_USER);
	// �������� ���� �� �������� ����������
	hMenuRU = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	hMenuEN = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
	// ����������� ���� � �������� ���� ����������
	SetMenu(hDialog, hMenuRU);
	static bool isClearEdit = true, isSave = false;
	SAVE = FALSE;
	return TRUE;
}

// ���������� ��������� WM_COMMAND ����� ������ ��� ������ ������ ����
void NotePadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_NEW:
		if (SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE)
		{
			if (MessageBox(hwnd, L"������ ��������� ���������?", L"����������", MB_YESNO) == 6)
			{
				saveFile(hwnd, 0);
				SendMessage(hEdit, WM_SETTEXT, 0, 0);
			}
			else{
				SendMessage(hEdit, WM_SETTEXT, 0, 0);
			}
		}
		break;
	case ID_OPEN:
		if (SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE)
		{
			if (MessageBox(hwnd, L"������ ��������� ���������?", L"����������", MB_YESNO) == 6)
			{
				saveFile(hwnd, 0);
				SendMessage(hEdit, WM_SETTEXT, 0, 0);
				openFile(hwnd);
				bIsOpenF = TRUE;
			}
			else{
				openFile(hwnd);
				bIsOpenF = TRUE;
			}
		}
		else{
			openFile(hwnd);
			bIsOpenF = TRUE;
		}
		break;
	case ID_SAVE:
		if (SendMessage(hEdit, EM_GETMODIFY, 0, 0))
		{
			saveFile(hwnd, 1);
		}
		break;
	case ID_SAVEAS:
		saveFile(hwnd, 0);
		break;
	case ID_RU:
		SetMenu(hDialog, hMenuRU);
		break;
	case ID_EN:
		SetMenu(hDialog, hMenuEN);
		break;
	case ID_UNDO:
		// ������� ��������� ��������
		SendMessage(hEdit, WM_UNDO, 0, 0);
		break;
	case ID_CUT:
		// ������ ���������� �������� ������ � ����� ������
		SendMessage(hEdit, WM_CUT, 0, 0);
		break;
	case ID_COPY:
		// ��������� ���������� �������� ������ � ����� ������
		SendMessage(hEdit, WM_COPY, 0, 0);
		break;
	case ID_PASTE:
		// ������� ����� � Edit Control �� ������ ������
		SendMessage(hEdit, WM_PASTE, 0, 0);
		break;
	case ID_DEL:
		// ������ ���������� �������� ������
		SendMessage(hEdit, WM_CLEAR, 0, 0);
		break;
	case ID_SELECTALL:
		// ������� ���� ����� � Edit Control
		SendMessage(hEdit, EM_SETSEL, 0, -1);
		break;
	case ID_STATUSBAR:
		// ���� ���� ����� TRUE, �� ������ ��������� ����������
		if (bShowStatusBar)
		{
			// ������� ���������� �������� ����
			HMENU hMenu = GetMenu(hDialog);
			// ������ ������� � ������ ���� "������ ���������"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
			// ������ ������ ���������
			ShowWindow(hStatus, SW_HIDE);
		}
		else
		{
			// ������� ���������� �������� ����
			HMENU hMenu = GetMenu(hDialog);
			// ��������� ������� �� ������ ���� "������ ���������"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
			// ��������� ������ ���������
			ShowWindow(hStatus, SW_SHOW);
		}
		bShowStatusBar = !bShowStatusBar;
	}
}

// ���������� ��������� WM_SIZE ����� ������ ��� ��������� �������� �������� ����
// ���� ��� ������������/�������������� �������� ����
void NotePadDlg::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rect1, rect2;
	// ������� ���������� ���������� ������� �������� ����
	GetClientRect(hDialog, &rect1);
	// ������� ���������� ������������ ������ ������ ���������
	SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2);
	// ��������� ����� ������� ���������� ����
	MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
	// ��������� ������ ������ ���������, 
	// ������ ������ ���������� ������� �������� ����
	SendMessage(hStatus, WM_SIZE, 0, 0);
}

// ���������� WM_INITMENUPOPUP ����� ������ ��������������� 
// ����� ������������ ������������ ����
void NotePadDlg::Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
	if (item == 0) // �������������� ����� ���� "������"
	{
		// ������� ������� ��������� ������
		DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0);
		WORD wBeginPosition = LOWORD(dwPosition);
		WORD wEndPosition = HIWORD(dwPosition);

		if (wEndPosition != wBeginPosition) // ������� �� �����?
		{
			// ���� ������� ���������� �����, 
			// �� ������� ������������ ������ ���� "����������", "��������" � "�������"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			// ���� ����������� ���������� �����, 
			// �� ������� ������������ ������ ���� "����������", "��������" � "�������"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_GRAYED);
		}

		if (IsClipboardFormatAvailable(CF_TEXT)) // ������� �� ����� � ������ ������?
			// ���� ������� ����� � ������ ������, 
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_ENABLED);
		else
			// ���� ����������� ����� � ������ ������, 
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_GRAYED);

		// ���������� �� ����������� ������ ���������� ��������?
		if (SendMessage(hEdit, EM_CANUNDO, 0, 0))
			// ���� ���������� ����������� ������ ���������� ��������,
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_ENABLED);
		else
			// ���� ����������� ����������� ������ ���������� ��������,
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_GRAYED);

		// ��������� ����� ������ � Edit Control
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		// ������� �� ���� ����� � Edit Control?
		if (length != wEndPosition - wBeginPosition)
			//���� �� ���� ����� ������� � Edit Control,
			// �� ������� ����������� ����� ���� "�������� ��"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_ENABLED);
		else
			// ���� ������� ���� ����� � Edit Control,
			// �� ������� ����������� ����� ���� "�������� ��"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_GRAYED);
	}
}

void NotePadDlg::Cls_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	if (flags & MF_POPUP) // ��������, �������� �� ���������� ����� ���� ���������� ����������� �������?
	{
		// ���������� ����� ���� �������� ���������� ����������� �������
		SendMessage(hStatus, SB_SETTEXT, 0, 0); // ������� ����� �� ������ ���������
	}
	else
	{
		// ���������� ����� ���� �������� �������� ������� (����� ���� "�������")
		TCHAR buf[200];
		// ������� ���������� �������� ���������� ����������
		HINSTANCE hInstance = GetModuleHandle(NULL);
		// ������� ������ �� ������� �����, ������������� � �������� ����������
		// ��� ���� ������������� ����������� ������ ������ ������������� �������������� ����������� ������ ����
		LoadString(hInstance, item, buf, 200);
		// ������� � ������ ��������� ����������� �������, ��������������� ����������� ������ ����
		SendMessage(hStatus, SB_SETTEXT, 0, LPARAM(buf));
	}
}

BOOL CALLBACK NotePadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
		HANDLE_MSG(hwnd, WM_INITMENUPOPUP, ptr->Cls_OnInitMenuPopup);
		HANDLE_MSG(hwnd, WM_MENUSELECT, ptr->Cls_OnMenuSelect);
	}
	return FALSE;
}

void NotePadDlg::saveFile(HWND hwnd, int i)
{
	/*TCHAR FullPath[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };
	open.hwndOwner = hwnd;
	open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0");
	open.lpstrFile = FullPath;
	open.nMaxFile = MAX_PATH;
	open.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	open.lpstrDefExt = TEXT("txt");
	if (GetSaveFileName(&open))
	{
		TCHAR* str = new TCHAR[open.nFileExtension - open.nFileOffset + 3];
		for (size_t i = open.nFileOffset; i <= open.nFileExtension + 3; i++)
		{
			str[i - open.nFileOffset] = open.lpstrFile[open.nFileOffset + i - open.nFileOffset];
		}
		HANDLE hFile = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		MessageBox(hwnd, str, TEXT("������ \"���������\""), MB_OK | MB_ICONINFORMATION);
		delete[] str;
		str = nullptr;
	}*/

	if (i)
	{

		TCHAR str[300] = TEXT("�� ������� ��� ���������� ����:\n");
		_tcscat_s(str, FullPat);
		/*MessageBox(hwnd, str, TEXT("������ \"���������\""), MB_OK | MB_ICONINFORMATION);*/

		std::wofstream fin;////������
		fin.open(FullPat);
		fin.imbue(std::locale(""));
		TCHAR text[65536];
		GetWindowText(hEdit, text, 200);
		if (!fin.is_open())
		{
			std::cerr << "File not open" << std::endl;
			return;
		}
		fin << text;
		fin.close();

	}
	else{
		TCHAR FullPath[MAX_PATH] = { 0 };
		OPENFILENAME open = { sizeof(OPENFILENAME) };
		open.hwndOwner = hwnd;
		open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0");
		open.lpstrFile = FullPath;
		open.nMaxFile = MAX_PATH;
		open.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
		open.lpstrDefExt = TEXT("txt");
		if (GetSaveFileName(&open))
		{
			SAVE = TRUE;
			TCHAR str[300] = TEXT("�� ������� ��� ���������� ����:\n");
			_tcscat_s(str, FullPath);
			/*	MessageBox(hwnd, str, TEXT("������ \"���������\""), MB_OK | MB_ICONINFORMATION);*/
			wsprintf(FullPat, FullPath);
		}

		std::wofstream fin;////������
		fin.open(FullPat);
		fin.imbue(std::locale(""));
		TCHAR text[65536] = { 0 };
		GetWindowText(hEdit, text, 200);

		//SendMessage(hEdit,EM_LINELENGTH,,);
		if (!fin.is_open())
		{
			std::cerr << "File not open" << std::endl;
			return;
		}
		fin << text;
		fin.close();
	}
}

void NotePadDlg::openFile(HWND hwnd)
{
	////TCHAR szCounterFileName[] = L"counter.dat";
	//TCHAR FullPath[MAX_PATH] = { 0 };
	//OPENFILENAME open = { sizeof(OPENFILENAME) };
	//open.hwndOwner = hwnd;
	//open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0");
	//open.lpstrFile = FullPath;
	//open.nMaxFile = MAX_PATH;
	//open.lpstrInitialDir = TEXT("C:\\");
	//open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	//if (GetOpenFileName(&open))
	//{
	//	_LARGE_INTEGER sizeFile;
	//	DWORD dwTemp;
	//	TCHAR* str = new TCHAR[open.nFileExtension - open.nFileOffset + 3];
	//	for (size_t i = open.nFileOffset; i <= open.nFileExtension + 3; i++)
	//	{
	//		str[i - open.nFileOffset] = open.lpstrFile[open.nFileOffset + i - open.nFileOffset];
	//	}
	//	HANDLE hFile = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//	GetFileSizeEx(hFile, &sizeFile);
	//	TCHAR* str1 = new TCHAR[sizeFile.QuadPart];
	//	ReadFile(hFile, str1, sizeof(str1), &dwTemp, NULL);
	//	SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, LPARAM(str1));
	//	//MessageBox(hwnd, str, TEXT("������ \"�������\""), MB_OK | MB_ICONINFORMATION);
	//	delete[] str1;
	//	delete[] str;
	//	str1 = nullptr;
	//	str = nullptr;
	//	CloseHandle(hFile);
	//
	//	////////
	//
	//	
	//}

	TCHAR FullPath[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };
	open.hwndOwner = hwnd;
	open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0");
	open.lpstrFile = FullPath;
	open.nMaxFile = MAX_PATH;
	open.lpstrInitialDir = TEXT("C:\\");
	open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (GetOpenFileName(&open))
	{
		TCHAR str[300] = TEXT("�� ������� ��� �������� ����:\n");
		_tcscat_s(str, FullPath);
		/*MessageBox(hwnd, str, TEXT("������ \"�������\""), MB_OK | MB_ICONINFORMATION);*/
		wsprintf(FullPat, FullPath);
	}

	std::wifstream fon;////��������� ������
	TCHAR text[100] = { 0 };
	TCHAR text2[65536] = { 0 };

	/*GetWindowText(hwnd, text, 200);*/
	fon.open(FullPat);
	fon.imbue(std::locale(""));
	if (!fon.is_open())
	{
		std::cerr << "File not open" << std::endl;
		return;
	}

	while (1)
	{
		fon.getline(text, 200);
		if (!lstrcmp(text, TEXT("\0")))
			break;
		_tcscat_s(text2, text);
		_tcscat_s(text2, TEXT("\r\n"));
	}

	SetWindowText(hEdit, text2);
	fon.close();
}