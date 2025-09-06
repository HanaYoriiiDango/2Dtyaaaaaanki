#include <windows.h>
#include <wingdi.h> 
#include <vector>

/*
________________________________________�������______________________________________________________________
������ ������� ����������� ����� �������� �� ������� �����, ��� ������ ������ ��� �����, ������� ���� �������� ��� �������������.
����� ������� � ����� �������� � ��������� ���������� � � ����� ���������� �� ����� ������������� �������.
��������� ������� ���� �������� ��� ������ ������ ������������ ������� ��������, ����� ����� �� �������� � ������� � ������ ���� ������
�������������.� ����������� �� ���� ������ ������ ����� ����������������, ����� ������������ ������ ��� ������������� �� ���������.
������ ����� ����������� ����������� ������������� ������� �������. 
���������, ��� �������� ������� ��������� ����� �������� �� ��, ����� ������ ��� ����������, � �������� �����������.
����� �� ��� �������� �������/������� ����� ���� ������������� ������������� ������� ������� � ���� �������?




*/

using namespace std;
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ������� wWinMain, � �� main
//configuration::advanced::character set - not set - ���� ���������� � ���������� 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

struct Player { // ������ ��� ������� ����������
    float x, y, width, height, speed;
    int current_room;
    HBITMAP hBitmap;//����� � ������� 
};

struct Object_sprite { // ��� ����������� ������� ��������� (no active)
    float x, y, width, height;
    bool collision = true;
    HBITMAP hBitmap;//����� � ������� 
};

struct Portal_sprite { // ������� ��� ����������� ������ ����� ���������
    bool is_locked = false;
    int target_room; // ������ ������� �������
    float x, y, width, height;
    HBITMAP hBitmap;//����� � ������� 
};

struct Location {
    HBITMAP hBackR;// ����� ��� �������� �����������
    vector<Portal_sprite> portals;
    vector<Object_sprite> object;

};

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
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

    
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� ������� e

    InitElements.InitRoom("forestR0.bmp");// 1 room
    InitElements.InitRoom("forestR1.bmp");// 2 room

    InitPortal(0, false, window.width / (float)1.2, window.height / (float)2.2, 256, 256, 1, "portal64pxR0.bmp");// 0 --> 1
    InitPortal(1, false, window.width / (float)19.2, window.height / (float)2.2, 256, 256, 0, "portal64pxR1.bmp");// 1 --> 0

    InitObject(0, window.width / (float)2.3, window.height / (float)2.9, 160, 210, true, "scarecrow.bmp"); // scarecrow
    InitObject(1, window.width / (float)1.2, window.height / (float)1.5, 128, 70, true, "chest128px.bmp"); // chest

    InitHeroes(0, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "gena128px.bmp"); // ���� ���������� � ����� �������
    //InitHeroes(1, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "valera128px.bmp"); // ������ ���������� �� ����� ����

}













void ObjectCollision(int NumRoom, int NumObject) {

    if (Rooms[NumRoom].object[NumObject].collision) {

        float positionX = Hero.x;
        float positionY = Hero.y;

        bool collisionHandled = false;
        const int line = (int)Rooms[NumRoom].object[NumObject].width, column = (int)Rooms[NumRoom].object[NumObject].height;

        for (int i = 0; i < line; i++) {
            for (int j = 0; j < column; j++) {

                if (!collisionHandled) { // ��������� ������ ���� ������������ ��� �� ����������

                    if (
                        Hero.x + Hero.width >= Rooms[NumRoom].object[NumObject].x
                        && Hero.x <= Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width
                        && Hero.y + Hero.height >= Rooms[NumRoom].object[NumObject].y
                        && Hero.y <= Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height ) {

                        float Left = (Hero.x + Hero.width) - Rooms[NumRoom].object[NumObject].x; // ���������� �� ����� ������� �����
                        float Right = (Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width) - (Hero.x - Hero.width); // ���������� �� ������ ������� �����
                        float Top = (Hero.y + Hero.height) - Rooms[NumRoom].object[NumObject].y; // ���������� �� ������� ������� �����
                        float Bottom = (Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height) - (Hero.y - Hero.height); // ���������� �� ������ ������� �����

                        //// ������� ����������� ���������� �������
                        float minOverlap = Left; // ������������, ��� ����������� �������� � overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// �������� ����������� ���� � ����������� �� ������� ������������
                        if (minOverlap == Left) {
                            Hero.x -= Hero.speed; // ����� ����� ������ �� ����� ������� �� ������� 
                            Hero.y = positionY + 1; // ������ ���������� 

                        }
                        else if (minOverlap == Right) { // ������������ ����������� � ���������?
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

                        collisionHandled = true; // ������������ ����������
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

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������e
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapObject);// �������� ����������� bitmap � �������� ������rger 
    
    if (hOldbm) // ���� �� ���� ������, ���������� ������
    {
        GetObject(hBitmapObject, sizeof(BITMAP), (LPSTR)&bm); // ���������� ������� �����������

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(255, 255, 255));//��� ������� ������ ����� ����� ��������������� ��� ����������

        }
        else
        {
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteObject(hOldbm);
    DeleteDC(hMemDC); // ������� �������� ������
    ReleaseDC(window.hWnd, hMemDC);
}

void ShowPortal(int NumRoom, int NumPortal) {

    int RoomX = (int)Rooms[NumRoom].portals[NumPortal].x;
    int RoomY = (int)Rooms[NumRoom].portals[NumPortal].y;
    int RoomWidth = (int)Rooms[NumRoom].portals[NumPortal].width;
    int RoomHeight = (int)Rooms[NumRoom].portals[NumPortal].height;
    HBITMAP PortalSprite = Rooms[NumRoom].portals[NumPortal].hBitmap;

    ShowBitmap(window.context, RoomX, RoomY, RoomWidth, RoomHeight, PortalSprite, true);// ������ 

}

void ShowObject(int NumRoom, int NumObject) {

    int ObjectX = (int)Rooms[NumRoom].object[NumObject].x;
    int ObjectY = (int)Rooms[NumRoom].object[NumObject].y;
    int ObjectWidth = (int)Rooms[NumRoom].object[NumObject].width;
    int ObjectHeight = (int)Rooms[NumRoom].object[NumObject].height;
    HBITMAP ObjectSprite = Rooms[NumRoom].object[NumObject].hBitmap;

    ShowBitmap(window.context, ObjectX, ObjectY, ObjectWidth, ObjectHeight, ObjectSprite, true);// enemy
    ObjectCollision(NumRoom, NumObject); // �������� �������

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

    ShowBitmap(window.context, 0, 0, window.width, window.height, Rooms[NumRoom].hBackR, false);//������ ���
    ShowPortal(NumRoom, 0);
    ShowObject(NumRoom, 0);
    ShowHero();

}


void LimitHero()
{
    Hero.x = max(Hero.x, window.width / 300); // ����������� ������ 
    Hero.x = min(Hero.x, window.width * 0.95); // ���������� ������
    Hero.y = max(Hero.y, window.height / 2.4); // ����������� ������
    Hero.y = min(Hero.y, window.height / 1.5); // ���������� �����

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

                if (!collisionHandled) { // ��������� ������ ���� ������������ ��� �� ����������

                    if (
                        HeroPositionX + HeroWidth >= PortalPositionX &&
                        HeroPositionX <= PortalPositionX + PortalWidth &&
                        HeroPositionY + HeroHeight >= PortalPositionY &&
                        HeroPositionY <= PortalPositionY + PortalHeight) {

                        float Left = (HeroPositionX + HeroWidth) - PortalPositionX; // ���������� �� ����� ������� �����
                        float Right = (PortalPositionX + PortalWidth) - (HeroPositionX - HeroWidth); // ���������� �� ������ ������� �����
                        float Top = (HeroPositionY + HeroHeight) - PortalPositionY; // ���������� �� ������� ������� �����
                        float Bottom = (PortalPositionY + PortalHeight) - (HeroPositionY - HeroHeight); // ���������� �� ������ ������� �����

                        //// ������� ����������� ���������� �������
                        float minOverlap = Left; // ������������, ��� ����������� �������� � overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// �������� ����������� ���� � ����������� �� ������� ������������
                        if (minOverlap == Left) {

                            TPControl(PortalTarget);
                            Hero.x = window.width / (float)2.3;
                            Hero.y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Right) { // ������������ ����������� � ���������?

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

                    collisionHandled = true; // ������������ ����������
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

    ProcessInput();//����� ����������
    LimitHero();//���������, ����� ������ ����� �� ������ �� ������� ������
    //ChangeHero();
    RunPortal(PortalNum);


}

void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
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

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����
    mciSendString(TEXT("play ..\\2Dtyaaaaaanki\\8-bitm.mp3 repeat"), NULL, 0, NULL);
    ShowCursor(NULL);

    while (!GetAsyncKeyState(VK_ESCAPE) && Game.active)
    {
        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR1) {

            ShowRoom(0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
            Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
            ProcessGame(0);



        }

        //ClearRoom(Rooms[0].hBackR, Rooms[0].portals[0].hBitmap, Rooms[0].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);

        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR2) {

            ShowRoom(1);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
            Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
            ProcessGame(0);



        }

        //ClearRoom(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);
    }

    ClearGame(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Hero.hBitmap);
}
