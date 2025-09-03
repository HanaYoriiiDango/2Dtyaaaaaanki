#include <windows.h>
#include <wingdi.h> 
//#include <algorithm> 
#include <vector>

using namespace std;
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) - ������� wWinMain, � �� main
//configuration::advanced::character set - not set - ���� ���������� � ���������� 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

// ������ ������ ����  
struct Hero_sprite { // ������ ��� ������� ����������
    float x, y, width, height, speed;
    int current_room;
    HBITMAP hBitmap;//����� � ������� 
};

struct Object_sprite { // ��� ����������� ������� ��������� (no active)
    float x, y, width, height;
    bool collision = false;
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

struct Player {
    vector<Hero_sprite> Hero;

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
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� ������� e

    InitRoom(0, "forestR0.bmp"); // 1 room
    InitRoom(1, "forestR1.bmp"); // 2 room

    InitPortal(0, false, window.width / (float)1.2, window.height / (float)2.2, 256, 256, 1, "portal64pxR0.bmp"); // 0 --> 1
    InitPortal(1, false, window.width / (float)19.2, window.height / (float)2.2, 256, 256, 0, "portal64pxR1.bmp"); // 1 --> 0

    InitObject(0, window.width / (float)2.3, window.height / (float)2.9, 160, 210, true, "scarecrow.bmp"); // scarecrow
    InitObject(1, window.width / (float)1.2, window.height / (float)1.5, 128, 70, true, "chest128px.bmp"); // chest

    InitHeroes(0, window.width / (float)14, window.height / (float)1.8, 128, 128, 20, 0, "gena128px.bmp"); // ���� ���������� � ����� �������
    //InitHeroes(Valera, window.width / 14, window.height / 1.8, 128, 128, 20, 0, "valera128px.bmp"); // ������ ���������� � ����� ������� ww

}

void ObjectCollision(int NumRoom, int NumObject, int NumList, int HeroID) {

    if (Rooms[NumRoom].object[NumObject].collision) {

        float positionX = Heroes_list[NumList].Hero[HeroID].x;
        float positionY = Heroes_list[NumList].Hero[HeroID].y;

        bool collisionHandled = false;
        const int line = (int)Rooms[NumRoom].object[NumObject].width, column = (int)Rooms[NumRoom].object[NumObject].height;

        for (int i = 0; i < line; i++) {
            for (int j = 0; j < column; j++) {

                if (!collisionHandled) { // ��������� ������ ���� ������������ ��� �� ����������

                    if (
                        Heroes_list[NumList].Hero[HeroID].x + Heroes_list[NumList].Hero[HeroID].width >= Rooms[NumRoom].object[NumObject].x
                        && Heroes_list[NumList].Hero[HeroID].x <= Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width
                        && Heroes_list[NumList].Hero[HeroID].y + Heroes_list[NumList].Hero[HeroID].height >= Rooms[NumRoom].object[NumObject].y
                        && Heroes_list[NumList].Hero[HeroID].y <= Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height ) {

                        float Left = (Heroes_list[NumList].Hero[HeroID].x + Heroes_list[NumList].Hero[HeroID].width) - Rooms[NumRoom].object[NumObject].x; // ���������� �� ����� ������� �����
                        float Right = (Rooms[NumRoom].object[NumObject].x + Rooms[NumRoom].object[NumObject].width) - (Heroes_list[NumList].Hero[HeroID].x - Heroes_list[NumList].Hero[HeroID].width); // ���������� �� ������ ������� �����
                        float Top = (Heroes_list[NumList].Hero[HeroID].y + Heroes_list[NumList].Hero[HeroID].height) - Rooms[NumRoom].object[NumObject].y; // ���������� �� ������� ������� �����
                        float Bottom = (Rooms[NumRoom].object[NumObject].y + Rooms[NumRoom].object[NumObject].height) - (Heroes_list[NumList].Hero[HeroID].y - Heroes_list[NumList].Hero[HeroID].height); // ���������� �� ������ ������� �����

                        //// ������� ����������� ���������� �������
                        float minOverlap = Left; // ������������, ��� ����������� �������� � overlapLeft
                        if (Right < minOverlap) minOverlap = Right;
                        if (Top < minOverlap) minOverlap = Top;
                        if (Bottom < minOverlap) minOverlap = Bottom;

                        //// �������� ����������� ���� � ����������� �� ������� ������������
                        if (minOverlap == Left) {
                            Heroes_list[NumList].Hero[HeroID].x -= Heroes_list[NumList].Hero[HeroID].speed; // ����� ����� ������ �� ����� ������� �� ������� 
                            Heroes_list[NumList].Hero[HeroID].y = positionY + 1; // ������ ���������� 

                        }
                        else if (minOverlap == Right) { // ������������ ����������� � ���������?
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

                        collisionHandled = true; // ������������ ����������, ������ �� ��������� ������ �����
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

void ShowObject(int NumRoom, int NumObject, int NumList, int HeroID) {

    int ObjectX = (int)Rooms[NumRoom].object[NumObject].x;
    int ObjectY = (int)Rooms[NumRoom].object[NumObject].y;
    int ObjectWidth = (int)Rooms[NumRoom].object[NumObject].width;
    int ObjectHeight = (int)Rooms[NumRoom].object[NumObject].height;
    HBITMAP ObjectSprite = Rooms[NumRoom].object[NumObject].hBitmap;

    ShowBitmap(window.context, ObjectX, ObjectY, ObjectWidth, ObjectHeight, ObjectSprite, true);// enemy
    ObjectCollision(NumRoom, NumObject, NumList, HeroID); // �������� �������

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

    ShowBitmap(window.context, 0, 0, window.width, window.height, Rooms[NumRoom].hBackR, false);//������ ���
    ShowPortal(NumRoom, 0);
    ShowObject(NumRoom, 0, NumList, HeroID);
    ShowHero( NumList, HeroID);

}


void LimitHero(int NumList, int HeroID)
{
    Heroes_list[NumList].Hero[HeroID].x = max(Heroes_list[NumList].Hero[HeroID].x, window.width / 300); // ����������� ������ 
    Heroes_list[NumList].Hero[HeroID].x = min(Heroes_list[NumList].Hero[HeroID].x, window.width * 0.95); // ���������� ������
    Heroes_list[NumList].Hero[HeroID].y = max(Heroes_list[NumList].Hero[HeroID].y, window.height / 2.4); // ����������� ������
    Heroes_list[NumList].Hero[HeroID].y = min(Heroes_list[NumList].Hero[HeroID].y, window.height / 1.5); // ���������� �����

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

                            TPControl(NumList, HeroID, PortalTarget);
                            Heroes_list[NumList].Hero[HeroID].x = window.width / (float)2.3;
                            Heroes_list[NumList].Hero[HeroID].y = window.height / (float)3.4;

                        }
                        else if (minOverlap == Right) { // ������������ ����������� � ���������?

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

                    collisionHandled = true; // ������������ ����������, ������ �� ��������� ������ �����
                    return;
                }
            }
        }
    }
 }

void ProcessGame(int NumList, int HeroID, int PortalNum) {

    ProcessInput(NumList, HeroID);//����� ����������
    LimitHero(NumList, HeroID);//���������, ����� ������ ����� �� ������ �� ������� ������
    RunPortal(NumList, HeroID, PortalNum);


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

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����
    mciSendString(TEXT("play ..\\2Dtyaaaaaanki\\8-bitm.mp3 repeat"), NULL, 0, NULL);
    ShowCursor(NULL);

    while (!GetAsyncKeyState(VK_ESCAPE) && Game.active)
    {
        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR1) {

            ShowRoom(0, 0, 0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
            Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
            ProcessGame(0, 0, 0);



        }

        //ClearRoom(Rooms[0].hBackR, Rooms[0].portals[0].hBitmap, Rooms[0].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);

        while (!GetAsyncKeyState(VK_ESCAPE) && Game.activeR2) {

            ShowRoom(1, 0, 0);
            BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
            Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
            ProcessGame(0, 0, 0);



        }

        //ClearRoom(Rooms[1].hBackR, Rooms[1].portals[0].hBitmap, Rooms[1].object[0].hBitmap, Heroes_list[0].Hero[0].hBitmap);
    }

    ClearGame();
}
