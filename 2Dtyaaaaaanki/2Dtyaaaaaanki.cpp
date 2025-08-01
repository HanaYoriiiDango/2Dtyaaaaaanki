#include <windows.h>
#include <gdiplus.h> // Для работы с изображениями
#pragma comment(lib, "gdiplus.lib") // Автоматическая линковка библиотеки

// Глобальные переменные
HBITMAP g_hBackground = NULL; // Хэндл для фонового изображения

// Обработчик событий окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        // Освобождаем ресурсы изображения при закрытии окна
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

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 1. Получаем размеры клиентской области
        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        // 2. Создаем контекст в памяти для рисования
        HDC hdcMem = CreateCompatibleDC(hdc);

        if (g_hBackground) {
            // 3. Выбираем наше изображение в контекст памяти
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, g_hBackground);

            // 4. Копируем изображение на экран (растягиваем на весь экран)
            StretchBlt(
                hdc,        // Контекст назначения (экран)
                0, 0,       // X, Y назначения
                width,      // Ширина назначения
                height,     // Высота назначения
                hdcMem,     // Контекст источника (память)
                0, 0,       // X, Y источника
                width,      // Ширина источника
                height,     // Высота источника
                SRCCOPY     // Просто копируем
            );

            // 5. Восстанавливаем старый битмап
            SelectObject(hdcMem, hOldBmp);
        }
        else {
            // Если изображение не загружено, рисуем черный фон
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        }

        // 6. Очищаем ресурсы
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Функция загрузки изображения
HBITMAP LoadBackgroundImage(const wchar_t* filename) {
    // Используем GDI+ для загрузки изображения
    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(filename);
    if (!bitmap || bitmap->GetLastStatus() != Gdiplus::Ok) {
        delete bitmap;
        return NULL;
    }

    HBITMAP hBitmap = NULL;
    bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &hBitmap);
    delete bitmap;
    return hBitmap;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    // Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 1. Регистрация класса окна
    const wchar_t CLASS_NAME[] = L"GameWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // 2. Получаем размеры экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 3. Создаем полноэкранное окно без рамки
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Game Window",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        screenWidth,
        screenHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) return 0;

    // 4. Загружаем фоновое изображение
    g_hBackground = LoadBackgroundImage(L"tyanki.bmp"); // Измените на свой файл

    // 5. Показываем окно
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hwnd);

    // 6. Основной цикл сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Завершение GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}

//// Шаг 1: Подключаем необходимые заголовочные файлы
//#include <windows.h>  // Основной заголовочный файл Windows API
//
//// Шаг 2: Объявляем оконную процедуру (функцию обработки сообщений)
//// Эта функция будет обрабатывать все сообщения, которые Windows отправляет нашему окну
//LRESULT CALLBACK WindowProc(
//    HWND hwnd,      // Дескриптор окна, к которому пришло сообщение
//    UINT uMsg,      // Идентификатор сообщения (например, нажатие клавиши)
//    WPARAM wParam,  // Дополнительная информация о сообщении
//    LPARAM lParam   // Дополнительная информация о сообщении
//)
//{
//    // Обрабатываем сообщения в зависимости от их типа
//    switch (uMsg)
//    {
//        // Сообщение WM_DESTROY приходит, когда окно закрывается
//    case WM_DESTROY:
//        // Отправляем сообщение WM_QUIT, чтобы завершить цикл сообщений
//        PostQuitMessage(0);
//        return 0;  // Возвращаем 0, сообщение обработано
//
//        // Сообщение WM_KEYDOWN приходит при нажатии клавиши на клавиатуре
//    case WM_KEYDOWN:
//        // Проверяем, была ли нажата клавиша ESC (VK_ESCAPE)
//        if (wParam == VK_ESCAPE)
//            // Закрываем окно
//            DestroyWindow(hwnd);
//        return 0;  // Возвращаем 0, сообщение обработано
//    }
//
//    // Для всех необработанных сообщений вызываем стандартную обработку Windows
//    return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//
//// Шаг 3: Главная функция программы (точка входа)
//// WINAPI - соглашение о вызове функций в Windows
//int WINAPI wWinMain(
//    HINSTANCE hInstance,      // Дескриптор текущего экземпляра приложения
//    HINSTANCE hPrevInstance,  // Дескриптор предыдущего экземпляра (всегда NULL в современных Windows)
//    PWSTR pCmdLine,           // Командная строка запуска (широкие символы)
//    int nCmdShow              // Флаг, указывающий как отображать окно (свернуто/развернуто)
//)
//{
//    // Шаг 4: Регистрируем класс окна
//    // Класс окна - это шаблон, на основе которого Windows создает окна
//
//    // Уникальное имя для нашего класса окна (в формате Unicode)
//    const wchar_t CLASS_NAME[] = L"MyFirstWindowClass";
//
//    // Заполняем структуру с параметрами класса окна
//    WNDCLASSW wc = {};  // Инициализируем всю структуру нулями
//
//    wc.lpfnWndProc = WindowProc;    // Указываем нашу функцию обработки сообщений
//    wc.hInstance = hInstance;       // Дескриптор нашего приложения
//    wc.lpszClassName = CLASS_NAME;  // Имя нашего класса окна
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // Загружаем стандартный курсор
//
//    // Регистрируем наш класс окна в системе
//    RegisterClassW(&wc);
//
//    // Шаг 5: Получаем размеры экрана
//    int screenWidth = GetSystemMetrics(SM_CXSCREEN);   // Ширина экрана в пикселях
//    int screenHeight = GetSystemMetrics(SM_CYSCREEN);  // Высота экрана в пикселях
//
//    // Шаг 6: Создаем само окно
//    HWND hwnd = CreateWindowExW(
//        0,                      // Дополнительные стили окна (без особенностей)
//        CLASS_NAME,             // Имя класса, которое мы зарегистрировали
//        L"My Fullscreen Window", // Заголовок окна (то, что будет в верхней строке)
//        WS_POPUP,               // Стиль окна: POPUP - без рамки и заголовка
//        0,                      // Позиция X (левый край)
//        0,                      // Позиция Y (верхний край)
//        screenWidth,            // Ширина окна = ширина экрана
//        screenHeight,           // Высота окна = высота экрана
//        NULL,                   // Родительское окно (нет)
//        NULL,                   // Меню (нет)
//        hInstance,              // Дескриптор нашего приложения
//        NULL                    // Дополнительные параметры (нет)
//    );
//
//    // Проверяем, создалось ли окно успешно
//    if (hwnd == NULL)
//        return 0;  // Если не создалось, завершаем программу
//
//    // Шаг 7: Показываем окно
//    ShowWindow(hwnd, SW_SHOW);  // Делаем окно видимым
//    UpdateWindow(hwnd);          // Принудительно обновляем окно (отправляем WM_PAINT)
//
//    // Шаг 8: Цикл обработки сообщений
//    // Это сердце любого Windows-приложения
//
//    MSG msg = {};  // Структура для хранения сообщения
//
//    // GetMessage извлекает сообщения из очереди
//    // Возвращает 0 только при получении WM_QUIT
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        // Преобразует сообщения виртуальных клавиш в символы
//        TranslateMessage(&msg);
//        // Отправляет сообщение в нашу оконную процедуру (WindowProc)
//        DispatchMessage(&msg);
//    }
//
//    // Возвращаем код завершения программы
//    return 0;
//}