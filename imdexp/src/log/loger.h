#pragma	once

class Loger
{
public:
	~Loger();
	static INT_PTR  CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool	Create(HINSTANCE hinstance, bool dos_window);
	void	Destroy();
	void	Print(TCHAR *str);
	void	Printf(TCHAR *str, ...);											
	void	EnableOk(bool enable = true);
	void	Show(bool show = true);
	static Loger	&Get()
	{
		static Loger singleton;
		return singleton;
	}
protected:
	bool	_dos_window;
	Loger();
	HWND	_hwnd;
};   
