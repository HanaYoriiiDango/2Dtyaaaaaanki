#include <windows.h>
#include <wingdi.h> 
#include <vector>

/*
________________________________________ЗАМЕТКИ______________________________________________________________
Единая система регистрации будет состоять из массива чисел, шде каждая ячейка это номер, занятый этим обьектом для инициализации.
Таким образом я смогу избежать в программе повторений и у разны элеиментов не будет повторяющихся номеров.
Программа должнга сама понимать при вызове метода иницализации игровог элемента, какой номер он занимает в системе и какого типа обьект
инициализации.В зависимости от того какорй обьект будет инициалиазирован, будут выставляться разные его характеристик по умолчанию.
Однако будет возможность настраивать характеристик каждого обьекта. 
Нанпример, при создании комнаты программа будет смотреть на то, какие номера уже существуют, и назнчать последующий.
Можно ли при созданни портала/обьекта через рект автоматически устанавливать размеры спрайта в этой системе?




*/

using namespace std;
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ожидает wWinMain, а не main
//configuration::advanced::character set - not set - могу обращаться к структурам 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

struct Player { // спрайт для игровых персонажей
    float x, y, width, height, speed;
    int current_room;
    HBITMAP hBitmap;//хэндл к спрайту 
};

struct Object_sprite { // для неподвижных игровых элементов (no active)
    float x, y, width, height;
    bool collision = true;
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

Location Rooms[10];

class Init {


};

class InitElGame {
private:
    int counter = 0, Element;
    vector <int> NumRoom;
    LPCSTR FileName;
     
public:
    void InitRoom(LPCSTR FileName) {

        counter++;
        NumRoom.push_back({ counter });

        for (int i = 0; i < NumRoom.size(); i++) {

            if (i == NumRoom.size() - 1) {

                Element = (NumRoom.size() - 1);
                Rooms[Element].hBackR = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            }
        }
    }
};

Player Hero;
InitElGame InitElements;



void InitPortal(int NumRoom, bool is_locked, float x, float y, float width, float height, int tg, LPCSTR FileName) {

    Rooms[NumRoom].portals.push_back({ is_locked, tg, x, y, width, height, (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });

}

void InitObject(int NumRoom, float x, float y, float width, float height, bool collision, LPCSTR FileName) {

    Rooms[NumRoom].object.push_back({ x, y, width, height, collision, (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) });

}

void InitHeroes(int NumList, float x, float y, float width, float height, float speed, int current_room, LPCSTR FileName) {

    Hero.x = x; 
    Hero.y = y;
    Hero.width = width;
    Hero.height = height; 
    Hero.speed = speed;
    Hero.current_room = current_room; 
    Hero.hBitmap = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

}

void InitGame()
{

    
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов e

    InitElements.InitRoom("forestR0.bmp");// 1 room
    InitElements.InitRoom("forestR1.bmp");// 2 room

    InitPortal(0, false, window.width / (float)1.2, window.height / (float)2.2, 256, 256, 1, "portal64pxR0.bmp");// 0 --> 1
    InitPortal(1, false, window.width / (float)19.2, window.height / (float)2.2, 256, 256, 0, "portal64pxR1.bmp");// 1 --> 0

    InitObject(0, window.width / (float)2.3, window.height / (float)2.9, 160, 210, true, "scarecrow.bmp"); // scarecrow
    InitObject(1, window.width / (float)1.2, window.height / (float)1.5, 128, 70, true, "chest128px.bmp"); // chest

    InitHeroes(0, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "gena128px.bmp"); // гена появляется с левой стороны
    //InitHeroes(1, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "valera128px.bmp"); // Валера появляется на месте гены

}













void ObjectCollision(int NumRoom, int NumObject) {

    if (Rooms[NumRoom].object[NumObject].collision) {

        float positionX = Hero.x;
        float positionY = Hero.y;

        bool collisionHandled = false;
        const int line = (int)Rooms[NumRoom].object[NumObject].width, column = (int)Rooms[NumRoom].object[NumObject].height;

        for (int i = 0; i < line; i++) {
            for (int j = 0; j < column; j++) {

                if (!collisionHandled) { // Проверяем только если столкновение ещё не обработано

                    if (
                        Hero.x + Hero.width >= Rooms[NumRoom].object[NumObject].x
                        && Hero.x <= Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width
                        && Hero.y + Hero.height >= Rooms[NumRoom].object[NumObject].y
                        && Hero.y <= Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height ) {

                        float Left = (Hero.x + Hero.width) - Rooms[NumRoom].object[NumObject].x; // расстояние до левой стороны блока
                        float Right = (Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width) - (Hero.x - Hero.width); // расстояние до правой стороны блока
                        float Top = (Hero.y + Hero.height) - Rooms[NumRoom].object[NumObject].y; // расстояние до верхней стороны блока
                        float Bottom = (Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height) - (Hero.y - Hero.height); // расстояние до нижней стороны блока

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

                        collisionHandled = true; // Столкновение обработано
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

void ShowObject(int NumRoom, int NumObject) {

    int ObjectX = (int)Rooms[NumRoom].object[NumObject].x;
    int ObjectY = (int)Rooms[NumRoom].object[NumObject].y;
    int ObjectWidth = (int)Rooms[NumRoom].object[NumObject].width;
    int ObjectHeight = (int)Rooms[NumRoom].object[NumObject].height;
    HBITMAP ObjectSprite = Rooms[NumRoom].object[NumObject].hBitmap;

    ShowBitmap(window.context, ObjectX, ObjectY, ObjectWidth, ObjectHeight, ObjectSprite, true);// enemy
    ObjectCollision(NumRoom, NumObject); // коллизия обьекта

}

void ShowHero() {

    int HeroX = (int)Hero.x;
    int HeroY = (int)Hero.y;
    int HeroWidth = (int)Hero.width;
    int HeroHeight = (int)Hero.height;
    HBITMAP HeroSprite = Hero.hBitmap;

    ShowBitmap(window.context, HeroX, HeroY, HeroWidth, HeroHeight, HeroSprite, true);// enemy

}

void ShowRoom(int NumRoom) {

    ShowBitmap(window.context, 0, 0, window.width, window.height, Rooms[NumRoom].hBackR, false);//задний фон
    ShowPortal(NumRoom, 0);
    ShowObject(NumRoom, 0);
    ShowHero();

}


void LimitHero()
{
    Hero.x = max(Hero.x, window.width / 300); // максимально сСЛЕВА 
    Hero.x = min(Hero.x, window.width * 0.95); // минимально справа
    Hero.y = max(Hero.y, window.height / 2.4); // максимально сверху
    Hero.y = min(Hero.y, window.height / 1.5); // минимально снизу

}

void TPControl(int Target) {

    if (Hero.current_room == 0) {
        if (Game.activeR1) {

            Hero.current_room = Target;
            Game.activeR1 = false;
            Game.activeR2 = true;

        }
    }
    else if (Hero.current_room == 1) {

        if (Game.activeR2) {

            Hero.current_room = Target;
            Game.activeR2 = false;
            Game.activeR1 = true;

        }
    }



}

void RunPortal(int NumPortal) {

    if (!Rooms[Hero.current_room].portals[NumPortal].is_locked) {

        bool collisionHandled = false;
        int PortalTarget = Rooms[Hero.current_room].portals[NumPortal].target_room;
        int HeroCurrentRoom = Hero.current_room;

        float HeroPositionX = Hero.x;
        float HeroPositionY = Hero.y;
        float HeroWidth = Hero.width;
        float HeroHeight = Hero.height;

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

                            TPControl(PortalTarget);
                            Hero.x = window.width / (float)2.3;
                            Hero.y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Right) { // диагональное перемещение в обработку?

                            TPControl(PortalTarget);
                            Hero.x = window.width / (float)2.3;
                            Hero.y = window.height / (float)3.4;
                        }
                        if (minOverlap == Top) {

                            TPControl(PortalTarget);
                            Hero.x = window.width / (float)2.3;
                            Hero.y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Bottom) {

                            TPControl(PortalTarget);
                            Hero.x = window.width / (float)2.3;
                            Hero.y = window.height / (float)3.4;

                        }
                    }

                    collisionHandled = true; // Столкновение обработано
                    return;
                }
            }
        }
    }
 }

//void ChangeHero() {
//
//    if () {
//
//
//
//
//    }
//
//
//
//}


void ProcessGame(int PortalNum) { // int NumRoom

    ProcessInput();//опрос клавиатуры
    LimitHero();//проверяем, чтобы спрайт героя не убежал за пределы экрана
    //ChangeHero();
    RunPortal(PortalNum);


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

void ClearGame(HBITMAP hBackR, HBITMAP portal, HBITMAP object, HBITMAP hero) {

    DeleteObject(hero);
    DeleteObject(object);
    DeleteObject(portal);
    DeleteObject(hBackR);

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

            ShowRoom(0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
            Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
            ProcessGame(0);



        }

        //ClearRoom(Rooms[0].hBackR, Rooms[0].portals[0].hBitmap, Rooms[0].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);

        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR2) {

            ShowRoom(1);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
            Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
            ProcessGame(0);



        }

        //ClearRoom(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);
    }

    ClearGame(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Hero.hBitmap);
}
