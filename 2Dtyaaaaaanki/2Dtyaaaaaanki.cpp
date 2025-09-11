#include <windows.h>
#include <algorithm>
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
    HBITMAP hBitmap;//хэндл к спрайту 
    bool collision = true;
    float x, y, width, height;
};

struct Portal_sprite { // обьекты для перемещения игрока между комнатами
    HBITMAP hBitmap;//хэндл к спрайту 
    bool is_locked = false;
    int target_room; // Индекс целевой комнаты
    float x, y, width, height;
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

} Game;

Location Rooms[10];
Player Hero;

class InitSystem {
private:

    int CounterRoom = 0;
    int CounterPortal = 0;
    int CounterObject = 0;

    vector <int> NumRoom;
    vector <int> NumPortal;
    vector <int> NumObject;
     
public:
    void Room(LPCSTR FileName) {

        int Element;

        CounterRoom++;
        NumRoom.push_back({ CounterRoom });

        for (int i = 0; i < NumRoom.size(); i++) {

            if (i == NumRoom.size() - 1) {

                Element = (NumRoom.size() - 1);
                Rooms[Element].hBackR = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            }
        }
    }
    void Portal(LPCSTR FileName, float x, float y) {

        int Element;

        CounterPortal++;
        NumPortal.push_back({ CounterPortal });

        for (int i = 0; i < NumRoom.size(); i++) {

            if (i == NumRoom.size() - 1) {

                Element = (NumRoom.size() - 1);

                HBITMAP Loading = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                BITMAP btmap; // автоматически определяю размеры спрайта 
                GetObject(Loading, sizeof(BITMAP), &btmap);

                int Target = TargetReturn(Element, CounterPortal);

                Rooms[Element].portals.push_back({ Loading, false, Target, x, y, (float)btmap.bmWidth, (float)btmap.bmHeight });

            }
        }
    }
    void Object(LPCSTR FileName, float x, float y) {

        int Element;

        CounterObject++;
        NumObject.push_back({ CounterObject });

        for (int i = 0; i < NumRoom.size(); i++) {

            if (i == NumRoom.size() - 1) {

                Element = (NumRoom.size() - 1);

                HBITMAP Loading = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                BITMAP btmap; // автоматически определяю размеры спрайта 
                GetObject(Loading, sizeof(BITMAP), &btmap);

                Rooms[Element].object.push_back({ Loading, true, x, y, (float)btmap.bmWidth, (float)btmap.bmHeight });

            }
        }
    }
    void Heroes(LPCSTR FileName, float x, float y) {

        Hero.hBitmap = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        BITMAP bitmap; // автоматически определяю размеры спрайта 
        GetObject(Hero.hBitmap, sizeof(BITMAP), &bitmap);

        Hero.x = x;
        Hero.y = y;
        Hero.width = bitmap.bmWidth;
        Hero.height = bitmap.bmHeight;
        Hero.speed = 20; // по умолчанию
        Hero.current_room = 0; // по умолчанию

    }
    bool Even(int num) {
        return (num & 1) == 0;
    }

    int TargetReturn(int room, int num2) {

        if (!Even(num2)) {
            return room + 1;
        }
        else {
            return room - 1;
        }
    }
};

InitSystem Init;

void InitGame()
{
   
    Init.Room("forestR0.bmp");// 1 room
    Init.Portal("portal64pxR0.bmp", window.width / (float)1.2, window.height / (float)2.2);
    Init.Object("scarecrow.bmp", window.width / (float)2.3, window.height / (float)2.9);

    Init.Room("forestR1.bmp");// 2 room
    Init.Portal("portal64pxR1.bmp", window.width / (float)19.2, window.height / (float)2.2);
    Init.Portal("portal64pxR2.bmp", window.width / (float)1.2, window.height / (float)2.2);
    Init.Object("chest128px.bmp", window.width / (float)2.3, window.height / (float)1.8);

    Init.Room("forestR2.bmp");// 3 room
    Init.Portal("portal64pxR3.bmp", window.width / (float)19.2, window.height / (float)2.2);


    Init.Heroes("gena128px.bmp", window.width / (float)14, window.height / (float)1.8);
    //Init.Heroes("valera128px.bmp", window.width / (float)14, window.height / (float)1.8);

}

void Collision() {

    for (auto object : Rooms[Hero.current_room].object) {

        if (Hero.x + Hero.width >= object.x && // left >= right
            Hero.x <= object.x + object.width && // 
            Hero.y <= object.y + object.height && // 
            Hero.y + Hero.height >= object.y) {  //

            // min возвращает мне мменьшее расстояние между сторонами - это и есть сторона коллизии
            float CoordX = min((Hero.x + Hero.width) - object.x, (object.x + object.width) - Hero.x);
            float CoordY = min((Hero.y + Hero.height) - object.y, (object.y + object.height) - Hero.y);

            if (CoordX < CoordY) { // если X больше то смотрим по X, нет - по Y
                if (Hero.x < object.x) Hero.x -= Hero.speed; // справа
                else  Hero.x += Hero.speed; // слева
            }
            else {
                if (Hero.y < object.y) Hero.y -= Hero.speed; // снизу
                else Hero.y += Hero.speed; // сверху
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

void ShowPortal() {

    for (auto portal : Rooms[Hero.current_room].portals) {

        ShowBitmap(window.context, portal.x, portal.y, portal.width, portal.height, portal.hBitmap, true);// портал 

    }
}

void ShowObject() {

    for (auto object : Rooms[Hero.current_room].object) {

        ShowBitmap(window.context, object.x, object.y, object.width, object.height, object.hBitmap, true);// enemy

    }
}

void ShowGame() {

    ShowBitmap(window.context, 0, 0, window.width, window.height, Rooms[Hero.current_room].hBackR, false);//задний фон
    ShowPortal();
    ShowObject();
    ShowBitmap(window.context, Hero.x, Hero.y, Hero.width, Hero.height, Hero.hBitmap, true);// hero

}


void LimitHero()
{
    Hero.x = max(Hero.x, window.width / 300); // максимально сСЛЕВА 
    Hero.x = min(Hero.x, window.width * 0.95); // минимально справа
    Hero.y = max(Hero.y, window.height / 2.4); // максимально сверху
    Hero.y = min(Hero.y, window.height / 1.5); // минимально снизу

}

void PortalRun() {

    for (auto portal : Rooms[Hero.current_room].portals) {

        if (Hero.x + Hero.width >= portal.x && // left >= right
            Hero.x <= portal.x + portal.width && // 
            Hero.y <= portal.y + portal.height && // 
            Hero.y + Hero.height >= portal.y) {  //

            // min возвращает мне мменьшее расстояние между сторонами - это и есть сторона коллизии
            float CoordX = min((Hero.x + Hero.width) - portal.x, (portal.x + portal.width) - Hero.x);
            float CoordY = min((Hero.y + Hero.height) - portal.y, (portal.y + portal.height) - Hero.y);

            if (CoordX < CoordY) { // если X больше то смотрим по X, нет - по Y
                if (Hero.x < portal.x) {
                
                    Hero.current_room = portal.target_room;
                    Hero.x = window.width / (float)5.2;
                
                }
                else {
                    
                    Hero.current_room = portal.target_room;
                    Hero.x = window.width / (float)1.3;
                
                }
            }
            else {
                if (Hero.y < portal.y) Hero.y -= Hero.speed; // снизу
                else Hero.y += Hero.speed; // сверху
            }
        }
    }
}

void TestDrawText() {

    SetTextColor(window.context, RGB(160, 160, 160));
    SetBkColor(window.context, RGB(0, 0, 0));
    SetBkMode(window.context, TRANSPARENT);
    //auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "CALIBRI");
    //auto hTmp = (HFONT)SelectObject(window.context, hFont);

    if (Hero.current_room == 2) {
        if (Hero.x == window.width / 2 && Hero.y == window.height / 2) {

            TextOutA(window.context, window.width / 2, window.height / 2, (LPCSTR)"Press to E", 8);


            //RECT rect = { 100, 100, 100, 100 };
            //DrawText(window.context, "Press to E", -1, &rect, DT_CENTER | DT_VCENTER);


            if (GetAsyncKeyState('E')) {

                TextOutA(window.context, window.width / 2, window.height / 2, (LPCSTR)"sosi<3", 6);

                //DrawText(window.context, "sosi", -1, &rect, DT_CENTER | DT_VCENTER);

            }
        }
    }
}

void ProcessGame() { 

    ProcessInput();//опрос клавиатуры
    //LimitHero();//проверяем, чтобы спрайт героя не убежал за пределы экрана
    Collision();
    PortalRun(); // перемещение через портал
    TestDrawText();

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

void Clear() { 
    // можно с помощью метода упростить очистку ресурсов???? 

    DeleteObject(Hero.hBitmap);
    DeleteObject(Rooms[2].portals[0].hBitmap);
    DeleteObject(Rooms[2].hBackR);

    DeleteObject(Rooms[1].object[0].hBitmap);
    DeleteObject(Rooms[1].portals[1].hBitmap);
    DeleteObject(Rooms[1].portals[0].hBitmap);
    DeleteObject(Rooms[1].hBackR);

    DeleteObject(Rooms[0].object[0].hBitmap);
    DeleteObject(Rooms[0].portals[0].hBitmap);
    DeleteObject(Rooms[0].hBackR);

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

    while (!GetAsyncKeyState(VK_ESCAPE) && Game.active) {

        ShowGame();
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
        Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
        ProcessGame();

    }

    Clear();
}
