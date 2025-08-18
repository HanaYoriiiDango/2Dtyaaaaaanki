#include <windows.h>
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ожидает wWinMain, а не main
//configuration::advanced::character set - not set - могу обращаться к структурам

struct {
	HWND hWnd;//хэндл окна - номерок
	HBITMAP hBackground, hBackBitmap; // хэндл для фонового изображения
	int width, height;//сюда сохраняем размеры окна 
} window;

void ShowWindow() { //  создаею функцию для демострации фона окна 


}

void InitGame() { // создаю функцию для инициализации всех объектов в игре

	// С помощью функции LoadImageA загружаю битмап для фона/спрайтоыв
	window.hBackground = (HBITMAP)LoadImageA(
		NULL,  // Дескриптор экземляра приложения, указываем откуда загружать. NULL - загрузка из внешнего файла
		"forest.bmp", // имя файла, относительный путь. Файл должен лежать рядом с exe
		IMAGE_BITMAP, // Тип изображения (IMAGE_BITMAP, IMAGE_ICON, IMAGE_CURSOR)
		0, 0,  // Желаемая ширина/высота (0 = оригинал)
		LR_LOADFROMFILE); // Флаг загрузки 

}

void InitWindow() { // Функция для инициализации окна

	window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

}

int APIENTRY wWinMain( // int WINAPI wWinMain() - тоже самое, но обычно используется для примеров
	_In_ HINSTANCE hInstance,      // экземпляр программы
	_In_opt_ HINSTANCE hPrevInstance, // прошлый экземпляр
	_In_ LPWSTR lpCmdLine,         // комадная строка
	_In_ int nCmdShow)             // флаги
{
	InitGame(); // инициализирую игровые объекты
	InitWindow(); // Инициализирую окно
	ShowCursor(NULL); // убрал курсор

	while (!GetAsyncKeyState(VK_ESCAPE)) { // Основной игровой цикл

		ShowWindow(); //рисую фон окна


	}
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

//// ШАГ 3, создаем окно
//window.hWnd = CreateWindow(
//	"edit",                              // имя класса - уникального шаблона окна
//	"My first window",					 // заголовок окна (не обязательно, можно просто 0)
//	WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, // стили окна: без рамки и заголовка | видимое окно | развернут на весь экран
//	0, // позиция X (int)
//	0, // позиция Y (int)
//	0, // ширина (int)
//	0, // высота (int)
//	0, // родитель 
//	0, // экземпляр 
//	0, // меню / ID 
//	0); // Доп. параметры (нет)