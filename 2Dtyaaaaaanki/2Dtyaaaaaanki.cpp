#include <windows.h>
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ������� wWinMain, � �� main
//configuration::advanced::character set - not set - ���� ���������� � ����������

struct {
	HWND hWnd;//����� ���� - �������
	HBITMAP hBackground, hBackBitmap; // ����� ��� �������� �����������
	int width, height;//���� ��������� ������� ���� 
} window;

void ShowWindow() { //  ������� ������� ��� ����������� ���� ���� 


}

void InitGame() { // ������ ������� ��� ������������� ���� �������� � ����

	// � ������� ������� LoadImageA �������� ������ ��� ����/���������
	window.hBackground = (HBITMAP)LoadImageA(
		NULL,  // ���������� ��������� ����������, ��������� ������ ���������. NULL - �������� �� �������� �����
		"forest.bmp", // ��� �����, ������������� ����. ���� ������ ������ ����� � exe
		IMAGE_BITMAP, // ��� ����������� (IMAGE_BITMAP, IMAGE_ICON, IMAGE_CURSOR)
		0, 0,  // �������� ������/������ (0 = ��������)
		LR_LOADFROMFILE); // ���� �������� 

}

void InitWindow() { // ������� ��� ������������� ����

	window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

}

int APIENTRY wWinMain( // int WINAPI wWinMain() - ���� �����, �� ������ ������������ ��� ��������
	_In_ HINSTANCE hInstance,      // ��������� ���������
	_In_opt_ HINSTANCE hPrevInstance, // ������� ���������
	_In_ LPWSTR lpCmdLine,         // �������� ������
	_In_ int nCmdShow)             // �����
{
	InitGame(); // ������������� ������� �������
	InitWindow(); // ������������� ����
	ShowCursor(NULL); // ����� ������

	while (!GetAsyncKeyState(VK_ESCAPE)) { // �������� ������� ����

		ShowWindow(); //����� ��� ����


	}
}






































//// ��� 1, ������� ���������� ���������� ��� �����������
//HBITMAP g_hBackground = NULL; // ����� ��� ����
//int g_winWidth = 0;            // ������ ����
//int g_winHeight = 0;           // ������ ����
//int g_imgWidth = 0;            // ������ �����������
//int g_imgHeight = 0;           // ������ �����������
//
//
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//
//	switch (uMsg) {
//
//	case WM_DESTROY:
//
//		// ��� 9 ������� ����������� ��� �������� ���������
//		if (g_hBackground) {
//			DeleteObject(g_hBackground);
//			g_hBackground = NULL;
//
//		}
//
//		PostQuitMessage(0);
//		return (0);
//
//	case WM_KEYDOWN:
//
//		if (wParam == VK_ESCAPE) 
//
//			DestroyWindow(hwnd);
//
//		return (0);
//
//		// ��� 6, ������ ������� �����������
//	case WM_PAINT:
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps); // ����� �������� ��������
//
//		// ������� ��� �����
//		HDC hdcMem = CreateCompatibleDC(hdc);
//		HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, g_winWidth, g_winHeight);
//		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);
//
//		// ���� ����� ������ �� ����� �������� ��� ������ 
//		RECT fullRect = { 0, 0, g_winWidth, g_winHeight };
//		FillRect(hdcMem, &fullRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
//
//		// ��� 7, ������������ ����������� ���� ��� ���������
//		if (g_hBackground) {
//			HDC hdcImg = CreateCompatibleDC(hdc);
//			HBITMAP hbmOldImg = (HBITMAP)SelectObject(hdcImg, g_hBackground);
//
//			SetStretchBltMode(hdcMem, COLORONCOLOR);
//			StretchBlt(
//				hdcMem, 0, 0, g_winWidth, g_winHeight, // ���� � ������ �������
//				hdcImg, 0, 0, g_imgWidth, g_imgHeight,  // ������ (������������ ������)
//
//				SRCCOPY                                 // ������� �����������
//			);
//
//			// ������� ��������� �������
//			SelectObject(hdcImg, hbmOldImg);
//			DeleteDC(hdcImg);
//
//		}
//
//		// ��� 8, ������� ����� �� ����� � ������� ��� �������
//		BitBlt(hdc, 0, 0, g_winWidth, g_winHeight, hdcMem, 0, 0, SRCCOPY);
//
//		// ������� 
//		SelectObject(hdcMem, hbmOld);
//		DeleteObject(hbmBuffer);
//		DeleteDC(hdcMem);
//
//		EndPaint(hwnd, &ps); // ����������� ��������
//
//		return (0);
//
//	}
//
//	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
//
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//	// ��� 2, ��������� ����������� � ������� bmp
//	g_hBackground = (HBITMAP)LoadImageW(
//		NULL,                   // ��� �������� �� �����
//		L"background.bmp", // ���� � ������ �����
//		IMAGE_BITMAP,           // ��� �����������
//		0, 0,                   // ��������������� �������
//		LR_LOADFROMFILE         // �������� �� �����
//	);
//
//	// �������� �� �������� �����������
//	if (!g_hBackground) {
//		MessageBox(NULL, L"������ �������� �����������", L"������", MB_ICONERROR);
//
//	}
//	// ��� 3, �������� ������ �����������
//	else {
//		BITMAP bm;
//		GetObject(g_hBackground, sizeof(bm), &bm);
//		g_imgWidth = bm.bmWidth; // ��������� ������
//		g_imgHeight = bm.bmHeight; // ��������� ������
//
//	}
//
//	const wchar_t CLASS_NAME[] = L"�������";
//
//	WNDCLASSW wc = {};
//
//	wc.lpfnWndProc = WindowProc;
//	wc.hInstance = hInstance;
//	wc.lpszClassName = CLASS_NAME;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//
//	RegisterClassW(&wc);
//
//	// ��� 4, ���������� ������ ����������������� ������ � ���������� ���������� 
//	int screenWidht = GetSystemMetrics(SM_CXSCREEN);
//	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//	g_winWidth = screenWidht; // ��������� �������
//	g_winHeight = screenHeight;
//
//
//	HWND hwnd = CreateWindowExW
//	(   0,
//		CLASS_NAME,
//		L"������� ",
//		WS_POPUP | WS_VISIBLE,
//		0,
//		0,
//		screenWidht,
//		screenHeight,
//		0,
//		0,
//		hInstance,
//		0
//	);
//		
//	if (hwnd == NULL) return (0);
//
//
//	UpdateWindow(hwnd);
//
//	MSG msg = {};
//
//	while (GetMessage(&msg, NULL, 0, 0)) {
//
//		TranslateMessage(&msg);
//		DispatchMessageW(&msg);
//
//	}
//
//	return (0);
//
//}

//// ��� 3, ������� ����
//window.hWnd = CreateWindow(
//	"edit",                              // ��� ������ - ����������� ������� ����
//	"My first window",					 // ��������� ���� (�� �����������, ����� ������ 0)
//	WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, // ����� ����: ��� ����� � ��������� | ������� ���� | ��������� �� ���� �����
//	0, // ������� X (int)
//	0, // ������� Y (int)
//	0, // ������ (int)
//	0, // ������ (int)
//	0, // �������� 
//	0, // ��������� 
//	0, // ���� / ID 
//	0); // ���. ��������� (���)