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
    float x, y, width, height, speed;
    int current_room;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Object_sprite { // для неподвижных игровых элементов (no active)
    float x, y, width, height;
    bool collision = false;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Portal_sprite { // обьекты для перемещения игрока между комнатами
    bool is_locked = false;
    int target_room; // Индекс целевой комнаты
    float x, y, width, height;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Location {
    HBITMAP hBackR;// хэндл для фонового изображения
    vector<Portal_sprite> portals;
    vector<Object_sprite> object;

};

struct Player {
    vector<Hero_sprite> Hero;

};

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

struct {
    bool active = true;
    bool activeR1 = true;
    bool activeR2 = true;

} Game;

Location Rooms[2];
Player Heroes_list[1];

void InitRoom(int NumRoom, LPCSTR FileName) {

    Rooms[NumRoom].hBackR = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

}

void InitPortal(int NumRoom, bool is_locked, float x, float y, float width, float height, int tg, LPCSTR FileName) {

    Rooms[NumRoom].portals.push_back({ is_locked, tg, x, y, width, height, (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)});

}

void InitObject(int NumRoom, float x, float y, float width, float height, bool collision, LPCSTR FileName) {

    Rooms[NumRoom].object.push_back({ x, y, width, height, collision, (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });

}

void InitHeroes(int NumList, float x, float y, float width, float height, float speed, int current_room, LPCSTR FileName) {

    Heroes_list[NumList].Hero.push_back({x, y, width, height, speed, current_room, (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)});

}

void InitGame()
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов e

    InitRoom(0, "forestR0.bmp"); // 1 room
    InitRoom(1, "forestR1.bmp"); // 2 room

    InitPortal(0, false, window.width / (float)1.2, window.height / (float)2.2, 256, 256, 1, "portal64pxR0.bmp"); // 0 --> 1
    InitPortal(1, false, window.width / (float)19.2, window.height / (float)2.2, 256, 256, 0, "portal64pxR1.bmp"); // 1 --> 0

    InitObject(0, window.width / (float)2.3, window.height / (float)2.9, 160, 210, true, "scarecrow.bmp"); // scarecrow
    InitObject(1, window.width / (float)1.2, window.height / (float)1.5, 128, 70, true, "chest128px.bmp"); // chest

    InitHeroes(0, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "gena128px.bmp"); // гена появляется с левой стороны
    //InitHeroes(Valera, window.width / 14, window.height / 1.8, 128, 128, 20, 0, "valera128px.bmp"); // Валера появляется с левой стороны ww

}

void ObjectCollision(int NumRoom, int NumObject, int NumList, int HeroID) {

    if (Rooms[NumRoom].object[NumObject].collision) {

        float positionX = Heroes_list[NumList].Hero[HeroID].x;
        float positionY = Heroes_list[NumList].Hero[HeroID].y;

        bool collisionHandled = false;
        const int line = (int)Rooms[NumRoom].object[NumObject].width, column = (int)Rooms[NumRoom].object[NumObject].height;

        for (int i = 0; i < line; i++) {
            for (int j = 0; j < column; j++) {

                if (!collisionHandled) { // Проверяем только если столкновение ещё не обработано

                    if (
                        Heroes_list[NumList].Hero[HeroID].x + Heroes_list[NumList].Hero[HeroID].width >= Rooms[NumRoom].object[NumObject].x
                        && Heroes_list[NumList].Hero[HeroID].x <= Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width
                        && Heroes_list[NumList].Hero[HeroID].y + Heroes_list[NumList].Hero[HeroID].height >= Rooms[NumRoom].object[NumObject].y
                        && Heroes_list[NumList].Hero[HeroID].y <= Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height ) {

                        float Left = (Heroes_list[NumList].Hero[HeroID].x + Heroes_list[NumList].Hero[HeroID].width) - Rooms[NumRoom].object[NumObject].x; // расстояние до левой стороны блока
                        float Right = (Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width) - (Heroes_list[NumList].Hero[HeroID].x - Heroes_list[NumList].Hero[HeroID].width); // расстояние до правой стороны блока
                        float Top = (Heroes_list[NumList].Hero[HeroID].y + Heroes_list[NumList].Hero[HeroID].height) - Rooms[NumRoom].object[NumObject].y; // расстояние до верхней стороны блока
                        float Bottom = (Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height) - (Heroes_list[NumList].Hero[HeroID].y - Heroes_list[NumList].Hero[HeroID].height); // расстояние до нижней стороны блока

                        //// Находим минимальное перекрытие вручную
                        float minOverlap = Left; // Предполагаем, что минимальное значение — overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// Изменяем направление мяча в зависимости от стороны столкновения
                        if (minOverlap == Left) {
                            Heroes_list[NumList].Hero[HeroID].x -= Heroes_list[NumList].Hero[HeroID].speed; // игрок будет стоять на месте скользя по обьекту 
                            Heroes_list[NumList].Hero[HeroID].y = positionY + 1; // эффект скольжения 

                        }
                        else if (minOverlap == Right) { // диагональное перемещение в обработку?
                            Heroes_list[NumList].Hero[HeroID].x += Heroes_list[NumList].Hero[HeroID].speed;
                            Heroes_list[NumList].Hero[HeroID].y = positionY - 1;

                        }
                        if (minOverlap == Top) {
                            Heroes_list[NumList].Hero[HeroID].y -= Heroes_list[NumList].Hero[HeroID].speed;
                            Heroes_list[NumList].Hero[HeroID].x = positionX + 1;

                        }
                        else if (minOverlap == Bottom) {
                            Heroes_list[NumList].Hero[HeroID].y += Heroes_list[NumList].Hero[HeroID].speed;
                            Heroes_list[NumList].Hero[HeroID].x = positionX - 1;

                        }
                    }

                        collisionHandled = true; // Столкновение обработано, больше не проверяем другие блоки
                        return;
                }
            }
        }
    }

}

void ProcessInput(int NumList, int HeroID)
{
    if (GetAsyncKeyState('W')) Heroes_list[NumList].Hero[HeroID].y -= Heroes_list[NumList].Hero[HeroID].speed;
    if (GetAsyncKeyState('A')) Heroes_list[NumList].Hero[HeroID].x -= Heroes_list[NumList].Hero[HeroID].speed;
    if (GetAsyncKeyState('S')) Heroes_list[NumList].Hero[HeroID].y += Heroes_list[NumList].Hero[HeroID].speed;
    if (GetAsyncKeyState('D')) Heroes_list[NumList].Hero[HeroID].x += Heroes_list[NumList].Hero[HeroID].speed;
        
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapObject, bool alpha = false)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // Создаем контекст памяти, совместимый с контекстом отображенияe
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapObject);// Выбираем изображение bitmap в контекст памятиrger 
    
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

void ShowPortal(int NumRoom, int NumPortal) {

    int RoomX = (int)Rooms[NumRoom].portals[NumPortal].x;
    int RoomY = (int)Rooms[NumRoom].portals[NumPortal].y;
    int RoomWidth = (int)Rooms[NumRoom].portals[NumPortal].width;
    int RoomHeight = (int)Rooms[NumRoom].portals[NumPortal].height;
    HBITMAP PortalSprite = Rooms[NumRoom].portals[NumPortal].hBitmap;

    ShowBitmap(window.context, RoomX, RoomY, RoomWidth, RoomHeight, PortalSprite, true);// портал 

}

void ShowObject(int NumRoom, int NumObject, int NumList, int HeroID) {

    int ObjectX = (int)Rooms[NumRoom].object[NumObject].x;
    int ObjectY = (int)Rooms[NumRoom].object[NumObject].y;
    int ObjectWidth = (int)Rooms[NumRoom].object[NumObject].width;
    int ObjectHeight = (int)Rooms[NumRoom].object[NumObject].height;
    HBITMAP ObjectSprite = Rooms[NumRoom].object[NumObject].hBitmap;

    ShowBitmap(window.context, ObjectX, ObjectY, ObjectWidth, ObjectHeight, ObjectSprite, true);// enemy
    ObjectCollision(NumRoom, NumObject, NumList, HeroID); // коллизия обьекта

}

void ShowHero(int NumList, int HeroID) {

    int HeroX = (int)Heroes_list[NumList].Hero[HeroID].x;
    int HeroY = (int)Heroes_list[NumList].Hero[HeroID].y;
    int HeroWidth = (int)Heroes_list[NumList].Hero[HeroID].width;
    int HeroHeight = (int)Heroes_list[NumList].Hero[HeroID].height;
    HBITMAP HeroSprite = Heroes_list[NumList].Hero[HeroID].hBitmap;

    ShowBitmap(window.context, HeroX, HeroY, HeroWidth, HeroHeight, HeroSprite, true);// enemy

}

void ShowRoom(int NumRoom, int NumList, int HeroID) {

    ShowBitmap(window.context, 0, 0, window.width, window.height, Rooms[NumRoom].hBackR, false);//задний фон
    ShowPortal(NumRoom, 0);
    ShowObject(NumRoom, 0, NumList, HeroID);
    ShowHero( NumList, HeroID);

}


void LimitHero(int NumList, int HeroID)
{
    Heroes_list[NumList].Hero[HeroID].x = max(Heroes_list[NumList].Hero[HeroID].x, window.width / 300); // максимально сСЛЕВА 
    Heroes_list[NumList].Hero[HeroID].x = min(Heroes_list[NumList].Hero[HeroID].x, window.width * 0.95); // минимально справа
    Heroes_list[NumList].Hero[HeroID].y = max(Heroes_list[NumList].Hero[HeroID].y, window.height / 2.4); // максимально сверху
    Heroes_list[NumList].Hero[HeroID].y = min(Heroes_list[NumList].Hero[HeroID].y, window.height / 1.5); // минимально снизу

}

void TPControl( int NumList, int HeroID, int Target) {

    if (Heroes_list[NumList].Hero[HeroID].current_room == 0) {
        if (Game.activeR1) {

            Heroes_list[NumList].Hero[HeroID].current_room = Target;
            Game.activeR1 = false;
            Game.activeR2 = true;

        }
    }
    else if (Heroes_list[NumList].Hero[HeroID].current_room == 1) {

        if (Game.activeR2) {

            Heroes_list[NumList].Hero[HeroID].current_room = Target;
            Game.activeR2 = false;
            Game.activeR1 = true;

        }
    }



}

void RunPortal(int NumList, int HeroID, int NumPortal) {

    if (!Rooms[Heroes_list[NumList].Hero[HeroID].current_room].portals[NumPortal].is_locked) {

        bool collisionHandled = false;
        int PortalTarget = Rooms[Heroes_list[NumList].Hero[HeroID].current_room].portals[NumPortal].target_room;
        int HeroCurrentRoom = Heroes_list[NumList].Hero[HeroID].current_room;

        float HeroPositionX = Heroes_list[NumList].Hero[HeroID].x;
        float HeroPositionY = Heroes_list[NumList].Hero[HeroID].y;
        float HeroWidth = Heroes_list[NumList].Hero[HeroID].width;
        float HeroHeight = Heroes_list[NumList].Hero[HeroID].height;

        float PortalPositionX = Rooms[HeroCurrentRoom].portals[NumPortal].x;
        float PortalPositionY = Rooms[HeroCurrentRoom].portals[NumPortal].y;
        const int PortalWidth = Rooms[HeroCurrentRoom].portals[NumPortal].width;
        const int PortalHeight = Rooms[HeroCurrentRoom].portals[NumPortal].height;

        for (int i = 0; i < PortalWidth; i++) {
            for (int j = 0; j < PortalHeight; j++) {

                if (!collisionHandled) { // Проверяем только если столкновение ещё не обработано

                    if (
                        HeroPositionX + HeroWidth >= PortalPositionX &&
                        HeroPositionX <= PortalPositionX + PortalWidth &&
                        HeroPositionY + HeroHeight >= PortalPositionY &&
                        HeroPositionY <= PortalPositionY + PortalHeight) {

                        float Left = (HeroPositionX + HeroWidth) - PortalPositionX; // расстояние до левой стороны блока
                        float Right = (PortalPositionX + PortalWidth) - (HeroPositionX - HeroWidth); // расстояние до правой стороны блока
                        float Top = (HeroPositionY + HeroHeight) - PortalPositionY; // расстояние до верхней стороны блока
                        float Bottom = (PortalPositionY + PortalHeight) - (HeroPositionY - HeroHeight); // расстояние до нижней стороны блока

                        //// Находим минимальное перекрытие вручную
                        float minOverlap = Left; // Предполагаем, что минимальное значение — overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// Изменяем направление мяча в зависимости от стороны столкновения
                        if (minOverlap == Left) {

                            TPControl(NumList, HeroID, PortalTarget);
                            Heroes_list[NumList].Hero[HeroID].x = window.width / (float)2.3;
                            Heroes_list[NumList].Hero[HeroID].y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Right) { // диагональное перемещение в обработку?

                            TPControl(NumList, HeroID, PortalTarget);
                            Heroes_list[NumList].Hero[HeroID].x = window.width / (float)2.3;
                            Heroes_list[NumList].Hero[HeroID].y = window.height / (float)3.4;
                        }
                        if (minOverlap == Top) {

                            TPControl(NumList, HeroID, PortalTarget);
                            Heroes_list[NumList].Hero[HeroID].x = window.width / (float)2.3;
                            Heroes_list[NumList].Hero[HeroID].y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Bottom) {

                            TPControl(NumList, HeroID, PortalTarget);
                            Heroes_list[NumList].Hero[HeroID].x = window.width / (float)2.3;
                            Heroes_list[NumList].Hero[HeroID].y = window.height / (float)3.4;

                        }
                    }

                    collisionHandled = true; // Столкновение обработано, больше не проверяем другие блоки
                    return;
                }
            }
        }
    }
 }

void ProcessGame(int NumList, int HeroID, int PortalNum) {

    ProcessInput(NumList, HeroID);//опрос клавиатуры
    LimitHero(NumList, HeroID);//проверяем, чтобы спрайт героя не убежал за пределы экрана
    RunPortal(NumList, HeroID, PortalNum);


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

void ClearRoom(HBITMAP hBackR, HBITMAP portal, HBITMAP object, HBITMAP hero) {

    DeleteObject(hero);
    DeleteObject(object);
    DeleteObject(portal);
    DeleteObject(hBackR);

}

void ClearGame() {

    DeleteObject(Heroes_list[0].Hero[0].hBitmap);
    ClearRoom(Rooms[0].hBackR, Rooms[0].portals[0].hBitmap, Rooms[0].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap); // Heroes_list[0].Hero[0].hBitmap
    ClearRoom(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);

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

    while (!GetAsyncKeyState(VK_ESCAPE) && Game.active)
    {
        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR1) {

            ShowRoom(0, 0, 0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
            Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
            ProcessGame(0, 0, 0);



        }

        //ClearRoom(Rooms[0].hBackR, Rooms[0].portals[0].hBitmap, Rooms[0].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);

        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR2) {

            ShowRoom(1, 0, 0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
            Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
            ProcessGame(0, 0, 0);



        }

        //ClearRoom(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);
    }

    ClearGame();
}
