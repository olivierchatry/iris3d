#include <stdafx.h>
Loger::Loger() : _hwnd(0)
{
}

Loger::~Loger()
{
	Destroy();
}

INT_PTR  CALLBACK Loger::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			Loger::Get().Destroy();
			break;
		}
		break;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		Loger::Get().Destroy();
		FreeConsole();
		break;
	default:
		return FALSE;
	}
	return TRUE;

}

void	Loger::Print(TCHAR *str)
{
	if (_hwnd == 0)
		return;
	HWND hwnd_edit = GetDlgItem(_hwnd, IDC_EDITLOG);
	std::string	text_full("");
	int text_length = GetWindowTextLength(hwnd_edit);
	TCHAR *text = new TCHAR [text_length + 1];
	SendMessage(hwnd_edit, WM_GETTEXT, (WPARAM)(text_length + 1), (LPARAM)text);

	text_full = text;
	text_full += str;
	text_full += "\r\n";
	SendMessage(hwnd_edit, WM_SETTEXT, (WPARAM)0, (LPARAM)text_full.c_str());
	if (_dos_window)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD	count;
		WriteConsole(handle, str, strlen(str), &count, 0);
		WriteConsole(handle, "\n", 1, &count, 0);
	}
	delete [] text;
}

void	Loger::Printf(TCHAR *str, ...)
{
	va_list args;
	TCHAR buffer[1024]; // FIXMEEE
	va_start(args, str);
	vsprintf(buffer, str, args);
	Print(buffer);
	va_end(args);
}

void	Loger::EnableOk(bool enable /* = true */)
{
	HWND hwnd_ok = GetDlgItem(_hwnd, IDOK);
	EnableWindow(hwnd_ok, (BOOL) enable);
}

bool	Loger::Create(HINSTANCE hinstance, bool dos_window)
{
	if (_hwnd == 0)
		_hwnd = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_DIALOGLOG), 0, DlgProc);
	Print("================================================");
	Printf("Welcome to ImdExp Console [build date %s %s]", __DATE__, __TIME__);
	Print("================================================");
	_dos_window = dos_window;
	if(_dos_window)
		AllocConsole();
	return (_hwnd > 0);
}  


void Loger::Show(bool show/*=true*/)
{
	if (_hwnd)
		ShowWindow(_hwnd, show ? SW_SHOW : SW_HIDE);
	HWND hwnd_edit = GetDlgItem(_hwnd, IDC_EDITLOG);
	ShowWindow(hwnd_edit, SW_SHOW);
}

void	Loger::Destroy()
{
	HWND	hwnd = _hwnd;
	_hwnd = 0;
	if (hwnd)
		EndDialog(hwnd, 0);
}

