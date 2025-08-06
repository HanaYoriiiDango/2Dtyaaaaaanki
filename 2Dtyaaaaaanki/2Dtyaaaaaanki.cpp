#include <windows.h>
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ожидает wWinMain, а не main
//configuration::advanced::character set - not set - хз зачем, но зато теперь могу обращаться к структурам

struct sprite {
	float x, y, width, height, speed;
	HBITMAP hBitmap;
};

sprite Enemy; 
sprite Player;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста (для буферизации)
    int width, height;//сюдв сохраняем размеры окна 
} window;

HBITMAP hBackground;// для фона 

void InitGame() { // здесь буду иницализировать все свойства игровых обьектов и спрайьты

    Enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "pugalo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Player.hBitmap = (HBITMAP)LoadImageA(NULL, "hero.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBackground = (HBITMAP)LoadImageA(NULL, "Forest.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


}

void ProcessSound() { // здесь буду проигрывать музыку




}

void LimitMoving() { // здесь буду ограничивать перемещение для игрока



}

void ProcessMoving() { // Функця для реализации перемещения игрока



}

void ShowBitmap(HDC hdc, HBITMAP hBitmapObject) {

    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hdc); // Создаем контекст памяти, совместимый с контекстом отображения 
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapObject);// Выбираем изображение bitmap в контекст памяти

}


void ShowObject() { // демонстрирую обьекты 



}

void InitWindow() { // инициализация для рисования игры



}


int WINAPI wWinMain() {

    InitWindow();
    InitGame();


}





































//// ШАГ 1, создаем глобальные переменные для изображения
//HBITMAP g_hBackground = NULL; // хэндл для фона
//int g_winWidth = 0;            // Ширина окна
//int g_winHeight = 0;           // Высота окна
//int g_imgWidth = 0;            // Ширина изображения
//int g_imgHeight = 0;           // Высота изображения
//
//
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//
//	switch (uMsg) {
//
//	case WM_DESTROY:
//
//		// ШАГ 9 удаляем изображение при закрытии программы
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
//		// ШАГ 6, делаем двойную буферизацию
//	case WM_PAINT:
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps); // здесь начинаем рисовать
//
//		// Создаем сам буфер
//		HDC hdcMem = CreateCompatibleDC(hdc);
//		HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, g_winWidth, g_winHeight);
//		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBuffer);
//
//		// если будут ошибки мы будем заливать фон черным 
//		RECT fullRect = { 0, 0, g_winWidth, g_winHeight };
//		FillRect(hdcMem, &fullRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
//
//		// ШАГ 7, расстягиваем изображение если оно загружено
//		if (g_hBackground) {
//			HDC hdcImg = CreateCompatibleDC(hdc);
//			HBITMAP hbmOldImg = (HBITMAP)SelectObject(hdcImg, g_hBackground);
//
//			SetStretchBltMode(hdcMem, COLORONCOLOR);
//			StretchBlt(
//				hdcMem, 0, 0, g_winWidth, g_winHeight, // Куда и какого размера
//				hdcImg, 0, 0, g_imgWidth, g_imgHeight,  // Откуда (оригинальный размер)
//
//				SRCCOPY                                 // Простое копирование
//			);
//
//			// Убираем временные объекты
//			SelectObject(hdcImg, hbmOldImg);
//			DeleteDC(hdcImg);
//
//		}
//
//		// ШАГ 8, выводим буфер на экран и очищаем его ресурсы
//		BitBlt(hdc, 0, 0, g_winWidth, g_winHeight, hdcMem, 0, 0, SRCCOPY);
//
//		// Очищаем 
//		SelectObject(hdcMem, hbmOld);
//		DeleteObject(hbmBuffer);
//		DeleteDC(hdcMem);
//
//		EndPaint(hwnd, &ps); // заканчиваем рисовать
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
//	// ШАГ 2, загружаем изображение в формате bmp
//	g_hBackground = (HBITMAP)LoadImageW(
//		NULL,                   // Для загрузки из файла
//		L"background.bmp", // ПУТЬ К ВАШЕМУ ФАЙЛУ
//		IMAGE_BITMAP,           // Тип изображения
//		0, 0,                   // Автоопределение размера
//		LR_LOADFROMFILE         // Загрузка из файла
//	);
//
//	// Проверка на загрузку изображения
//	if (!g_hBackground) {
//		MessageBox(NULL, L"Ошибка загрузки изображения", L"Ошибка", MB_ICONERROR);
//
//	}
//	// ШАГ 3, получаем размер изображения
//	else {
//		BITMAP bm;
//		GetObject(g_hBackground, sizeof(bm), &bm);
//		g_imgWidth = bm.bmWidth; // Сохраняем ширину
//		g_imgHeight = bm.bmHeight; // сохраняем высоту
//
//	}
//
//	const wchar_t CLASS_NAME[] = L"Широкие";
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
//	// ШАГ 4, записываем размер пользовательского экрана в глобальные переменные 
//	int screenWidht = GetSystemMetrics(SM_CXSCREEN);
//	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//	g_winWidth = screenWidht; // Сохраняем размеры
//	g_winHeight = screenHeight;
//
//
//	HWND hwnd = CreateWindowExW
//	(   0,
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