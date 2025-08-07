#include <windows.h>
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ожидает wWinMain, а не main
//configuration::advanced::character set - not set - могу обращаться к структурам

// Структура окна
struct {
	    HWND hWnd;//хэндл окна - номерок
	    int width, height;//сюда сохраняем размеры окна 
} window;

// ШАГ 1 Создаем битмап для фона 
HBITMAP hBack;

void InitWindow() { // Функция для инициализации окна

	window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

}

// ШАГ 2, создаем функцию для инициализации всех объектов в игре
void InitGame() { 

	hBack = (HBITMAP)LoadImageA(
		NULL,  // Дескриптор экземляра приложения, указываем откуда загружать. NULL - загрузка из внешнего файла
		"forest.bmp", // имя файла, относительный путь. Файл должен лежать рядом с exe
		IMAGE_BITMAP, // Тип изображения (IMAGE_BITMAP, IMAGE_ICON, IMAGE_CURSOR)
		0, 0,  // Желаемая ширина/высота (0 = оригинал)
		LR_LOADFROMFILE); // Флаг загрузки fffff

}

// Задаем точку входа программы
int APIENTRY wWinMain( // int WINAPI wWinMain() - тоже самое, но обычно используется для примеров
    _In_ HINSTANCE hInstance,      // экземпляр программы
	_In_opt_ HINSTANCE hPrevInstance, // прошлый экземпляр
	_In_ LPWSTR lpCmdLine,         // комадная строка
	_In_ int nCmdShow)             // флаги
{
	// Назначаем клавишу для завершения программы
	while (!GetAsyncKeyState(VK_ESCAPE)) {

		InitWindow(); // Инициализируем окно	

	}

}






































//struct sprite {
//	float x, y, width, height, speed;
//	HBITMAP hBitmap;
//    HDC frontDC, backDC;// два контекста (для буферизации)
//};
//
//sprite Enemy; 
//sprite Player;
//
//struct {
//    HWND hWnd;//хэндл окна
//    HBITMAP hBack; // бимап для заднего буфера
//    HDC frontDC, backDC;// два контекста (для буферизации)
//    int width, height;//сюдв сохраняем размеры окна 
//} window;
//
//struct {
//    bool action = true;
//
//} game;
//
//void InitGame() { // здесь буду иницализировать все свойства игровых обьектов и спрайьты
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
//void ProcessSound() { // здесь буду проигрывать музыку
//
//
//
//
//}
//
//void LimitMoving() { // здесь буду ограничивать перемещение для игрока
//
//
//
//}
//
//void ProcessBild() { // Функця для назначения клавиш
//
//    // перемещение игрока на WASD
//    if (GetAsyncKeyState('W')) Player.y -= Player.speed;
//    if (GetAsyncKeyState('S')) Player.y += Player.speed;
//    if (GetAsyncKeyState('A')) Player.x -= Player.speed;
//    if (GetAsyncKeyState('D')) Player.x += Player.speed;
//
//    if (game.action && GetAsyncKeyState(VK_ESCAPE)) {
//
//        // программа завершается, если нажать escape 
//        game.action = false;
//
//    }
//
//}
//
//void ShowWindow() {
//
//    // записываю размеры изображения 
//    BITMAP bm;
//    GetObject(window.hBack, sizeof(bm), &bm);
//    int imgWidth = bm.bmWidth; // Сохраняем ширину
//    int imgHeight = bm.bmHeight; // сохраняем высоту
//
//    window.frontDC = GetDC(window.hWnd);// Получаем контекст устройства (передний буфер)
//
//    window.backDC = CreateCompatibleDC(window.frontDC);// Создаем контекст в памяти (задний буфер)
//
//    HBITMAP hBackground = CreateCompatibleBitmap(window.frontDC, window.width, window.height); // Создаем битмап для заднего буфера
//
//    // по сути мы создаем сначало "холст" на котором рисуется само изображение, потом копируем его в буфер и вставляем между кадрами,
//    // это нужно чтобы избежать мерцания, чтобы пользователь не видел сам процесс прорисовки 
//
//    SelectObject(window.backDC, window.hBack); // Привязываем битмап к контексту памяти
//   
//    StretchBlt(
//        window.frontDC, 0, 0, window.width, window.height, // Куда и какого размера
//        window.backDC, 0, 0, bm.bmWidth, bm.bmHeight,  // Откуда (оригинальный размер)
//       SRCCOPY                                 // Простое копирование
//
//    );
//
//    DeleteObject(hBackground);
//    DeleteObject(window.backDC);
//    ReleaseDC(window.hWnd, window.frontDC);
//
//}
//
//void ShowObject() { // буферизацмя для спрайтов 
//
//
//
//
//
//}
//
//void InitWindow() { // инициализация для окна
//
//    SetProcessDPIAware();
//    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);
//
//    RECT r; // Получаем размеры окна
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
//    // общий игровой цикл
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