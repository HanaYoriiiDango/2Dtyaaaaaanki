#include <windows.h>
#include <wingdi.h> 
//#include <algorithm> 
#include <vector>

using namespace std;
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ожидает wWinMain, а не main
//configuration::advanced::character set - not set - могу обращаться к структурам 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

// секция данных игры  
struct Hero_sprite { // спрайт для игровых персонажей
    float x, y, width, height, rad, dx, dy, speed;
    bool collision = false;
    int current_room;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Object_sprite { // для неподвижных игровых элементов (no active)
    float x, y, width, height, rad, dx, dy, speed;
    bool collision = false;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Portal_sprite { // обьекты для перемещения игрока между комнатами
    bool is_locked = false;
    int target_room; // Индекс целевой комнаты
    float x, y, width, height, speed;
    bool collision = false;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Location {
    HBITMAP hBackR;// хэндл для фонового изображения
    vector<Portal_sprite> portals;
    vector<Object_sprite> object;

};

Location Room[1];
Hero_sprite Hero;

struct {
    int score, balls;//количество набранных очков и оставшихся "жизней"
    bool action = false;//состояние - ожидание (игрок должен нажать пробел) или игра f
} game;

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

void InitRoom(int NumRoom, HBITMAP hBackR) {

    Room[NumRoom].hBackR = hBackR;


}

void InitPortal(int NumRoom, int NumPortal, float x, float y, float width, float height, int tg, HBITMAP hBitmap) {

    Room[NumRoom].portals[NumPortal].x = x;
    Room[NumRoom].portals[NumPortal].y = y;
    Room[NumRoom].portals[NumPortal].width = width;
    Room[NumRoom].portals[NumPortal].height = height;
    Room[NumRoom].portals[NumPortal].target_room = tg;
    Room[NumRoom].portals[NumPortal].hBitmap = hBitmap;

}

void InitObject(int NumRoom, int NumObject, float x, float y, float width, float height, bool collision, HBITMAP hBitmap) {

    Room[NumRoom].object[NumObject].x = x;
    Room[NumRoom].object[NumObject].y = y;
    Room[NumRoom].object[NumObject].width = width;
    Room[NumRoom].object[NumObject].height = height;
    Room[NumRoom].object[NumObject].collision = collision;
    Room[NumRoom].object[NumObject].hBitmap = hBitmap;

}

void InitGame()
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов e
    Room[0].hBackR = (HBITMAP)LoadImageA(NULL, "forestR0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Room[1].hBackR = (HBITMAP)LoadImageA(NULL, "forestR1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    Hero.hBitmap = (HBITMAP)LoadImageA(NULL, "chel128px.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Hero.width = 128; // беру размеры изображения
    Hero.height = 128;
    Hero.speed = 20;//скорость перемещения персонажа
    Hero.x = window.width / 14;//герой появляется с левой стороны экрана
    Hero.y = window.height / 1.8 ;
    Hero.current_room = 0; // текущее положение

    game.score = 0;
    game.balls = 9;

    InitRoom();
    InitRoom();

    InitPortal(0, 0, window.width / 1.2, window.height / 2.5, 256, 256, 1, (HBITMAP)LoadImageA(NULL, "portal64pxR0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // 0 --> 1
    InitPortal(1, 0, window.width / 14, window.height / 1.8, 256, 256, 0, (HBITMAP)LoadImageA(NULL, "portal64pxR1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // 1 --> 0

    InitObject(0, 0, window.width / 2.3, window.height / 2.9, 160, 210, true, (HBITMAP)LoadImageA(NULL, "scarecrow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // scarecrow
    InitObject(1, 0, window.width / 1.9, window.height / 3.3, 32, 32, true, (HBITMAP)LoadImageA(NULL, "chest32px.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // chest



  
}

void ShowScore()
{
    //поиграем шрифтами и цветами
    SetTextColor(window.context, RGB(160, 160, 160));
    SetBkColor(window.context, RGB(0, 0, 0));
    SetBkMode(window.context, TRANSPARENT);
    auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "CALIBRI");
    auto hTmp = (HFONT)SelectObject(window.context, hFont);

    char txt[32];//буфер для текста
    _itoa_s(game.score, txt, 10);//преобразование числовой переменной в текст. текст окажется в переменной txt
    TextOutA(window.context, 10, 10, "Score", 5);
    TextOutA(window.context, 200, 10, (LPCSTR)txt, strlen(txt));

    _itoa_s(game.balls, txt, 10);
    TextOutA(window.context, 10, 100, "Balls", 5);
    TextOutA(window.context, 200, 100, (LPCSTR)txt, strlen(txt));
}

void ObjectCollision(object_sprite object) {

    if (object.collision) {

        float positionX = Hero.x;
        float positionY = Hero.y;

        bool collisionHandled = false;
        const int line = object.width, column = object.height;

        for (int i = 0; i < line; i++) {
            for (int j = 0; j < column; j++) {

                if (!collisionHandled) { // Проверяем только если столкновение ещё не обработано

                    if (Hero.x + Hero.width >= object.x && Hero.x <= object.x + object.width && // 
                        Hero.y + Hero.height >= object.y && Hero.y <= object.y + object.height) {

                        //float penetrationX = min(Hero.x + Hero.width - object.x, object.x + object.width - Hero.x);// глубина проникновения по осям
                        //float penetrationY = min(Hero.y + Hero.height - object.y, object.y + object.height - Hero.y); // это рассчет расстояния между обьектами

                        float Left = (Hero.x + Hero.width) - object.x; // расстояние до левой стороны блока
                        float Right = (object.x + object.width) - (Hero.x - Hero.width); // расстояние до правой стороны блока
                        float Top = (Hero.y + Hero.height) - object.y; // расстояние до верхней стороны блока
                        float Bottom = (object.y +object.height) - (Hero.y - Hero.height); // расстояние до нижней стороны блока

                        //// Находим минимальное перекрытие вручную
                        float minOverlap = Left; // Предполагаем, что минимальное значение — overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// Изменяем направление мяча в зависимости от стороны столкновения
                        if (minOverlap == Left) {
                            Hero.x -= Hero.speed; // игрок будет стоять на месте скользя по обьекту 
                            Hero.y = positionY + 1; // эффект скольжения 

                        }
                        else if (minOverlap == Right) { // диагональное перемещение в обработку?
                            Hero.x += Hero.speed;
                            Hero.y = positionY - 1;

                        }
                        if (minOverlap == Top) {
                            Hero.y -= Hero.speed;
                            Hero.x = positionX + 1;

                        }
                        else if (minOverlap == Bottom) {
                            Hero.y += Hero.speed;
                            Hero.x = positionX - 1;

                        }

                    }

                        collisionHandled = true; // Столкновение обработано, больше не проверяем другие блоки
                        return;
                }
            }
        }
    }

}

void ProcessInput()
{
    if (GetAsyncKeyState('W')) Hero.y -= Hero.speed;
    if (GetAsyncKeyState('A')) Hero.x -= Hero.speed;
    if (GetAsyncKeyState('S')) Hero.y += Hero.speed;
    if (GetAsyncKeyState('D')) Hero.x += Hero.speed;
        
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapObject, bool alpha = false)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // Создаем контекст памяти, совместимый с контекстом отображения
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapObject);// Выбираем изображение bitmap в контекст памяти

    if (hOldbm) // Если не было ошибок, продолжаем работу
    {
        GetObject(hBitmapObject, sizeof(BITMAP), (LPSTR)&bm); // Определяем размеры изображения

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(255, 255, 255));//все пиксели белого цвета будут интепретированы как прозрачные

        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // Рисуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// Восстанавливаем контекст памяти
    }

    DeleteObject(hOldbm);
    DeleteDC(hMemDC); // Удаляем контекст памяти
    ReleaseDC(window.hWnd, hMemDC);
}

void ShowLocation(Location location, portal_sprite portal, hero_sprite hero, object_sprite object) { 

        ShowBitmap(window.context, 0, 0, window.width, window.height, location.hBackR, false);//задний фон
        ShowBitmap(window.context, object.x, object.y, object.width, object.height, object.hBitmap, true);// enemy
        ShowBitmap(window.context, hero.x, hero.y, hero.width, hero.height, hero.hBitmap, true);// enemy
        ShowBitmap(window.context, portal.x, portal.y, portal.width, portal.height, portal.hBitmap, true);// портал
        ObjectCollision(object);

   
}


void LimitHero()
{
    Hero.x = max(Hero.x, window.width / 300); // максимально сСЛЕВА
    Hero.x = min(Hero.x, window.width * 0.95); // минимально справа
    Hero.y = max(Hero.y, window.height / 2.4); // максимально сверху
    Hero.y = min(Hero.y, window.height / 1.5); // минимально снизу

}

void ShowGame() {

    ShowBitmap(window.context, 0, 0, window.width, window.height, Room0.hBackR, false);//задний фон
    ShowBitmap(window.context, object.x, object.y, object.width, object.height, object.hBitmap, true);// enemy
    ShowBitmap(window.context, hero.x, hero.y, hero.width, hero.height, hero.hBitmap, true);// enemy
    ShowBitmap(window.context, portal.x, portal.y, portal.width, portal.height, portal.hBitmap, true);// портал
    ObjectCollision(object);

}

void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
    GetClientRect(window.hWnd, &r);

}

void clear() {

    DeleteObject(Chest.hBitmap);
    DeleteObject(Scarecrow.hBitmap);
    DeleteObject(Hero.hBitmap);
    DeleteObject(PortalLeft.hBitmap);
    DeleteObject(PortalRight.hBitmap);
    DeleteObject(Room1.hBackR);
    DeleteObject(Room0.hBackR);

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
 {

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры
    mciSendString(TEXT("play ..\\2Dtyaaaaaanki\\8-bitm.mp3 repeat"), NULL, 0, NULL);
    ShowCursor(NULL);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        ShowGame();//рисуем фон, героя и обьекты
        ShowScore();//рисуем хуню всякую
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)

        ProcessInput();//опрос клавиатуры
        LimitHero();//проверяем, чтобы спрайт героя не убежал за пределы экрана
        
    }

    clear();

}
