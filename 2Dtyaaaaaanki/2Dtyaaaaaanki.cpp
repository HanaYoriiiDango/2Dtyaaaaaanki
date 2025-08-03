#include <windows.h>

// Глобальные переменные для изображения
HBITMAP g_hBackground = NULL;  // Хэндл изображения
int g_winWidth = 0;            // Ширина окна
int g_winHeight = 0;           // Высота окна
int g_imgWidth = 0;            // Ширина изображения
int g_imgHeight = 0;           // Высота изображения

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        // Освобождаем ресурсы изображения
        if (g_hBackground) {
            DeleteObject(g_hBackground);
            g_hBackground = NULL;
        }
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            DestroyWindow(hwnd);
        return 0;

    case WM_SIZE:
        // Сохраняем новые размеры окна
        g_winWidth = LOWORD(lParam);
        g_winHeight = HIWORD(lParam);
        // Требуем перерисовки
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Двойная буферизация (см. объяснение ниже)
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, g_winWidth, g_winHeight);
        SelectObject(hdcMem, hbmBuffer);

        // Заливаем фон чёрным (на случай ошибки загрузки)
        FillRect(hdcMem, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Если изображение загружено - рисуем с растяжением
        if (g_hBackground) {
            HDC hdcImg = CreateCompatibleDC(hdc);
            SelectObject(hdcImg, g_hBackground);

            // Растягиваем изображение на всю область окна
            SetStretchBltMode(hdcMem, COLORONCOLOR); // Режим растяжения
            StretchBlt(
                hdcMem, 0, 0, g_winWidth, g_winHeight,    // Назначение
                hdcImg, 0, 0, g_imgWidth, g_imgHeight,    // Источник
                SRCCOPY                                   // Операция
            );
            DeleteDC(hdcImg);
        }

        // Копируем буфер на экран
        BitBlt(hdc, 0, 0, g_winWidth, g_winHeight, hdcMem, 0, 0, SRCCOPY);

        // Очистка ресурсов
        DeleteObject(hbmBuffer);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Широкие до хуя";

    // Загрузка фонового изображения
    g_hBackground = (HBITMAP)LoadImageW(
        NULL,
        L"tyanki.bmp", // Исправленный путь вв
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    if (!g_hBackground) {
        MessageBoxW(NULL,
            L"Ошибка загрузки изображения!\nПроверьте путь и формат файла.",
            L"Ошибка",
            MB_ICONERROR);
    }

    // Получаем размеры изображения
    if (g_hBackground) {
        BITMAP bm;
        GetObject(g_hBackground, sizeof(bm), &bm);
        g_imgWidth = bm.bmWidth;
        g_imgHeight = bm.bmHeight;
    }

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // Получаем размеры экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    g_winWidth = screenWidth; // Инициализируем размеры
    g_winHeight = screenHeight;

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Широкие",
        WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}






































//#include <windows.h>
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//
//	switch (uMsg) {
//
//		case WM_DESTROY:
//
//			PostQuitMessage(0);
//			return (0);
//
//		case WM_KEYDOWN:
//
//			if (wParam == VK_ESCAPE) 
//
//				DestroyWindow(hwnd);
//
//			return (0);
//
//	}
//
//	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
//
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//	const wchar_t CLASS_NAME[] = L"Широкие до хуя";
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
//	int screenWidht = GetSystemMetrics(SM_CXSCREEN);
//	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//
//	HWND hwnd = CreateWindowExW
//	(0,
//		CLASS_NAME,
//		L"Широкие ",
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