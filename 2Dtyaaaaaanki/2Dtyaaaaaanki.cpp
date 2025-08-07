#include <windows.h>
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ������� wWinMain, � �� main
//configuration::advanced::character set - not set - ���� ���������� � ����������

// ��������� ����
struct {
	    HWND hWnd;//����� ���� - �������
	    int width, height;//���� ��������� ������� ���� 
} window;

// ��� 1 ������� ������ ��� ���� 
HBITMAP hBack;

void InitWindow() { // ������� ��� ������������� ����

	window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

}

// ��� 2, ������� ������� ��� ������������� ���� �������� � ����
void InitGame() { 

	hBack = (HBITMAP)LoadImageA(
		NULL,  // ���������� ��������� ����������, ��������� ������ ���������. NULL - �������� �� �������� �����
		"forest.bmp", // ��� �����, ������������� ����. ���� ������ ������ ����� � exe
		IMAGE_BITMAP, // ��� ����������� (IMAGE_BITMAP, IMAGE_ICON, IMAGE_CURSOR)
		0, 0,  // �������� ������/������ (0 = ��������)
		LR_LOADFROMFILE); // ���� ��������

}

// ������ ����� ����� ���������
int APIENTRY wWinMain( // int WINAPI wWinMain() - ���� �����, �� ������ ������������ ��� ��������
    _In_ HINSTANCE hInstance,      // ��������� ���������
	_In_opt_ HINSTANCE hPrevInstance, // ������� ���������
	_In_ LPWSTR lpCmdLine,         // �������� ������
	_In_ int nCmdShow)             // �����
{
	// ��������� ������� ��� ���������� ���������
	while (!GetAsyncKeyState(VK_ESCAPE)) {

		InitWindow(); // �������������� ����	

	}

}






































//struct sprite {
//	float x, y, width, height, speed;
//	HBITMAP hBitmap;
//    HDC frontDC, backDC;// ��� ��������� (��� �����������)
//};
//
//sprite Enemy; 
//sprite Player;
//
//struct {
//    HWND hWnd;//����� ����
//    HBITMAP hBack; // ����� ��� ������� ������
//    HDC frontDC, backDC;// ��� ��������� (��� �����������)
//    int width, height;//���� ��������� ������� ���� 
//} window;
//
//struct {
//    bool action = true;
//
//} game;
//
//void InitGame() { // ����� ���� ��������������� ��� �������� ������� �������� � ��������
//
//    Enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "pugalo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    Player.hBitmap = (HBITMAP)LoadImageA(NULL, "hero.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    window.hBack = (HBITMAP)LoadImageA(NULL, "forest.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//    Player.width = 100;
//    Player.height = 100;
//    Player.speed = 30;
//    Player.x = window.width / 2;
//    Player.y = window.height / 2;
//
//
//
//}
//
//void ProcessSound() { // ����� ���� ����������� ������
//
//
//
//
//}
//
//void LimitMoving() { // ����� ���� ������������ ����������� ��� ������
//
//
//
//}
//
//void ProcessBild() { // ������ ��� ���������� ������
//
//    // ����������� ������ �� WASD
//    if (GetAsyncKeyState('W')) Player.y -= Player.speed;
//    if (GetAsyncKeyState('S')) Player.y += Player.speed;
//    if (GetAsyncKeyState('A')) Player.x -= Player.speed;
//    if (GetAsyncKeyState('D')) Player.x += Player.speed;
//
//    if (game.action && GetAsyncKeyState(VK_ESCAPE)) {
//
//        // ��������� �����������, ���� ������ escape 
//        game.action = false;
//
//    }
//
//}
//
//void ShowWindow() {
//
//    // ��������� ������� ����������� 
//    BITMAP bm;
//    GetObject(window.hBack, sizeof(bm), &bm);
//    int imgWidth = bm.bmWidth; // ��������� ������
//    int imgHeight = bm.bmHeight; // ��������� ������
//
//    window.frontDC = GetDC(window.hWnd);// �������� �������� ���������� (�������� �����)
//
//    window.backDC = CreateCompatibleDC(window.frontDC);// ������� �������� � ������ (������ �����)
//
//    HBITMAP hBackground = CreateCompatibleBitmap(window.frontDC, window.width, window.height); // ������� ������ ��� ������� ������
//
//    // �� ���� �� ������� ������� "�����" �� ������� �������� ���� �����������, ����� �������� ��� � ����� � ��������� ����� �������,
//    // ��� ����� ����� �������� ��������, ����� ������������ �� ����� ��� ������� ���������� 
//
//    SelectObject(window.backDC, window.hBack); // ����������� ������ � ��������� ������
//   
//    StretchBlt(
//        window.frontDC, 0, 0, window.width, window.height, // ���� � ������ �������
//        window.backDC, 0, 0, bm.bmWidth, bm.bmHeight,  // ������ (������������ ������)
//       SRCCOPY                                 // ������� �����������
//
//    );
//
//    DeleteObject(hBackground);
//    DeleteObject(window.backDC);
//    ReleaseDC(window.hWnd, window.frontDC);
//
//}
//
//void ShowObject() { // ����������� ��� �������� 
//
//
//
//
//
//}
//
//void InitWindow() { // ������������� ��� ����
//
//    SetProcessDPIAware();
//    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);
//
//    RECT r; // �������� ������� ����
//    GetClientRect(window.hWnd, &r);
//    window.width = r.right - r.left;
//    window.height = r.bottom - r.top;
//
//    ShowWindow();
//
//}
//
//
//
//int WINAPI wWinMain(
//     HINSTANCE hInstance,
//     HINSTANCE hPrevInstance,
//     LPWSTR    lpCmdLine,
//     int       nCmdShow) {
//
//    InitGame();
//    InitWindow();
//
//    // ����� ������� ����
//    while (game.action) {
//
//        ProcessBild();
//       
//
//
//
//    }
//
//}









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