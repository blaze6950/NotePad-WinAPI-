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
		if (MessageBox(hwnd, L"Хотите сохранить изменения?", L"Секундочку", MB_YESNO) == 6)
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
	// Получим дескриптор текстового поля
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	// Создадим строку состояния
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDialog, WM_USER);
	// Загрузим меню из ресурсов приложения
	hMenuRU = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	hMenuEN = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
	// Присоединим меню к главному окну приложения
	SetMenu(hDialog, hMenuRU);
	static bool isClearEdit = true, isSave = false;
	SAVE = FALSE;
	return TRUE;
}

// Обработчик сообщения WM_COMMAND будет вызван при выборе пункта меню
void NotePadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_NEW:
		if (SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE)
		{
			if (MessageBox(hwnd, L"Хотите сохранить изменения?", L"Секундочку", MB_YESNO) == 6)
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
			if (MessageBox(hwnd, L"Хотите сохранить изменения?", L"Секундочку", MB_YESNO) == 6)
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
		// Отменим последнее действие
		SendMessage(hEdit, WM_UNDO, 0, 0);
		break;
	case ID_CUT:
		// Удалим выделенный фрагмент текста в буфер обмена
		SendMessage(hEdit, WM_CUT, 0, 0);
		break;
	case ID_COPY:
		// Скопируем выделенный фрагмент текста в буфер обмена
		SendMessage(hEdit, WM_COPY, 0, 0);
		break;
	case ID_PASTE:
		// Вставим текст в Edit Control из буфера обмена
		SendMessage(hEdit, WM_PASTE, 0, 0);
		break;
	case ID_DEL:
		// Удалим выделенный фрагмент текста
		SendMessage(hEdit, WM_CLEAR, 0, 0);
		break;
	case ID_SELECTALL:
		// Выделим весь текст в Edit Control
		SendMessage(hEdit, EM_SETSEL, 0, -1);
		break;
	case ID_STATUSBAR:
		// Если флаг равен TRUE, то строка состояния отображена
		if (bShowStatusBar)
		{
			// Получим дескриптор главного меню
			HMENU hMenu = GetMenu(hDialog);
			// Снимем отметку с пункта меню "Строка состояния"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
			// Скроем строку состояния
			ShowWindow(hStatus, SW_HIDE);
		}
		else
		{
			// Получим дескриптор главного меню
			HMENU hMenu = GetMenu(hDialog);
			// Установим отметку на пункте меню "Строка состояния"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
			// Отобразим строку состояния
			ShowWindow(hStatus, SW_SHOW);
		}
		bShowStatusBar = !bShowStatusBar;
	}
}

// Обработчик сообщения WM_SIZE будет вызван при изменении размеров главного окна
// либо при сворачивании/восстановлении главного окна
void NotePadDlg::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rect1, rect2;
	// Получим координаты клиентской области главного окна
	GetClientRect(hDialog, &rect1);
	// Получим координаты единственной секции строки состояния
	SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2);
	// Установим новые размеры текстового поля
	MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
	// Установим размер строки состояния, 
	// равный ширине клиентской области главного окна
	SendMessage(hStatus, WM_SIZE, 0, 0);
}

// Обработчик WM_INITMENUPOPUP будет вызван непосредственно 
// перед активизацией всплывающего меню
void NotePadDlg::Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
	if (item == 0) // Активизируется пункт меню "Правка"
	{
		// Получим границы выделения текста
		DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0);
		WORD wBeginPosition = LOWORD(dwPosition);
		WORD wEndPosition = HIWORD(dwPosition);

		if (wEndPosition != wBeginPosition) // Выделен ли текст?
		{
			// Если имеется выделенный текст, 
			// то сделаем разрешёнными пункты меню "Копировать", "Вырезать" и "Удалить"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			// Если отсутствует выделенный текст, 
			// то сделаем недоступными пункты меню "Копировать", "Вырезать" и "Удалить"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_GRAYED);
		}

		if (IsClipboardFormatAvailable(CF_TEXT)) // Имеется ли текст в буфере обмена?
			// Если имеется текст в буфере обмена, 
			// то сделаем разрешённым пункт меню "Вставить"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_ENABLED);
		else
			// Если отсутствует текст в буфере обмена, 
			// то сделаем недоступным пункт меню "Вставить"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_GRAYED);

		// Существует ли возможность отмены последнего действия?
		if (SendMessage(hEdit, EM_CANUNDO, 0, 0))
			// Если существует возможность отмены последнего действия,
			// то сделаем разрешённым пункт меню "Отменить"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_ENABLED);
		else
			// Если отсутствует возможность отмены последнего действия,
			// то сделаем недоступным пункт меню "Отменить"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_GRAYED);

		// Определим длину текста в Edit Control
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		// Выделен ли весь текст в Edit Control?
		if (length != wEndPosition - wBeginPosition)
			//Если не весь текст выделен в Edit Control,
			// то сделаем разрешённым пункт меню "Выделить всё"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_ENABLED);
		else
			// Если выделен весь текст в Edit Control,
			// то сделаем недоступным пункт меню "Выделить всё"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_GRAYED);
	}
}

void NotePadDlg::Cls_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	if (flags & MF_POPUP) // Проверим, является ли выделенный пункт меню заголовком выпадающего подменю?
	{
		// Выделенный пункт меню является заголовком выпадающего подменю
		SendMessage(hStatus, SB_SETTEXT, 0, 0); // Убираем текст со строки состояния
	}
	else
	{
		// Выделенный пункт меню является конечным пунктом (пункт меню "команда")
		TCHAR buf[200];
		// Получим дескриптор текущего экземпляра приложения
		HINSTANCE hInstance = GetModuleHandle(NULL);
		// Зарузим строку из таблицы строк, расположенной в ресурсах приложения
		// При этом идентификатор загружаемой строки строго соответствует идентификатору выделенного пункта меню
		LoadString(hInstance, item, buf, 200);
		// Выводим в строку состояния контекстную справку, соответствующую выделенному пункту меню
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
		MessageBox(hwnd, str, TEXT("Диалог \"Сохранить\""), MB_OK | MB_ICONINFORMATION);
		delete[] str;
		str = nullptr;
	}*/

	if (i)
	{

		TCHAR str[300] = TEXT("Вы выбрали для сохранения файл:\n");
		_tcscat_s(str, FullPat);
		/*MessageBox(hwnd, str, TEXT("Диалог \"Сохранить\""), MB_OK | MB_ICONINFORMATION);*/

		std::wofstream fin;////Запись
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
			TCHAR str[300] = TEXT("Вы выбрали для сохранения файл:\n");
			_tcscat_s(str, FullPath);
			/*	MessageBox(hwnd, str, TEXT("Диалог \"Сохранить\""), MB_OK | MB_ICONINFORMATION);*/
			wsprintf(FullPat, FullPath);
		}

		std::wofstream fin;////Запись
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
	//	//MessageBox(hwnd, str, TEXT("Диалог \"Открыть\""), MB_OK | MB_ICONINFORMATION);
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
		TCHAR str[300] = TEXT("Вы выбрали для открытия файл:\n");
		_tcscat_s(str, FullPath);
		/*MessageBox(hwnd, str, TEXT("Диалог \"Открыть\""), MB_OK | MB_ICONINFORMATION);*/
		wsprintf(FullPat, FullPath);
	}

	std::wifstream fon;////Получение данных
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