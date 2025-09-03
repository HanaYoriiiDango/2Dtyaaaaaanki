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
    float x, y, width, height, rad, dx, dy, speed;
    bool collision = false;
    int current_room;
    HBITMAP hBitmap;//����� � ������� 
};

struct Object_sprite { // ��� ����������� ������� ��������� (no active)
    float x, y, width, height, rad, dx, dy, speed;
    bool collision = false;
    HBITMAP hBitmap;//����� � ������� 
};

struct Portal_sprite { // ������� ��� ����������� ������ ����� ���������
    bool is_locked = false;
    int target_room; // ������ ������� �������
    float x, y, width, height, speed;
    bool collision = false;
    HBITMAP hBitmap;//����� � ������� 
};

struct Location {
    HBITMAP hBackR;// ����� ��� �������� �����������
    vector<Portal_sprite> portals;
    vector<Object_sprite> object;

};

Location Room[1];
Hero_sprite Hero;

struct {
    int score, balls;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ���� f
} game;

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
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
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� ������� e
    Room[0].hBackR = (HBITMAP)LoadImageA(NULL, "forestR0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Room[1].hBackR = (HBITMAP)LoadImageA(NULL, "forestR1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    Hero.hBitmap = (HBITMAP)LoadImageA(NULL, "chel128px.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    Hero.width = 128; // ���� ������� �����������
    Hero.height = 128;
    Hero.speed = 20;//�������� ����������� ���������
    Hero.x = window.width / 14;//����� ���������� � ����� ������� ������
    Hero.y = window.height / 1.8 ;
    Hero.current_room = 0; // ������� ���������

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
    //�������� �������� � �������
    SetTextColor(window.context, RGB(160, 160, 160));
    SetBkColor(window.context, RGB(0, 0, 0));
    SetBkMode(window.context, TRANSPARENT);
    auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "CALIBRI");
    auto hTmp = (HFONT)SelectObject(window.context, hFont);

    char txt[32];//����� ��� ������
    _itoa_s(game.score, txt, 10);//�������������� �������� ���������� � �����. ����� �������� � ���������� txt
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

                if (!collisionHandled) { // ��������� ������ ���� ������������ ��� �� ����������

                    if (Hero.x + Hero.width >= object.x && Hero.x <= object.x + object.width && // 
                        Hero.y + Hero.height >= object.y && Hero.y <= object.y + object.height) {

                        //float penetrationX = min(Hero.x + Hero.width - object.x, object.x + object.width - Hero.x);// ������� ������������� �� ����
                        //float penetrationY = min(Hero.y + Hero.height - object.y, object.y + object.height - Hero.y); // ��� ������� ���������� ����� ���������

                        float Left = (Hero.x + Hero.width) - object.x; // ���������� �� ����� ������� �����
                        float Right = (object.x + object.width) - (Hero.x - Hero.width); // ���������� �� ������ ������� �����
                        float Top = (Hero.y + Hero.height) - object.y; // ���������� �� ������� ������� �����
                        float Bottom = (object.y +object.height) - (Hero.y - Hero.height); // ���������� �� ������ ������� �����

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

                        collisionHandled = true; // ������������ ����������, ������ �� ��������� ������ �����
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

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapObject);// �������� ����������� bitmap � �������� ������

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

void ShowLocation(Location location, portal_sprite portal, hero_sprite hero, object_sprite object) { 

        ShowBitmap(window.context, 0, 0, window.width, window.height, location.hBackR, false);//������ ���
        ShowBitmap(window.context, object.x, object.y, object.width, object.height, object.hBitmap, true);// enemy
        ShowBitmap(window.context, hero.x, hero.y, hero.width, hero.height, hero.hBitmap, true);// enemy
        ShowBitmap(window.context, portal.x, portal.y, portal.width, portal.height, portal.hBitmap, true);// ������
        ObjectCollision(object);

   
}


void LimitHero()
{
    Hero.x = max(Hero.x, window.width / 300); // ����������� ������
    Hero.x = min(Hero.x, window.width * 0.95); // ���������� ������
    Hero.y = max(Hero.y, window.height / 2.4); // ����������� ������
    Hero.y = min(Hero.y, window.height / 1.5); // ���������� �����

}

void ShowGame() {

    ShowBitmap(window.context, 0, 0, window.width, window.height, Room0.hBackR, false);//������ ���
    ShowBitmap(window.context, object.x, object.y, object.width, object.height, object.hBitmap, true);// enemy
    ShowBitmap(window.context, hero.x, hero.y, hero.width, hero.height, hero.hBitmap, true);// enemy
    ShowBitmap(window.context, portal.x, portal.y, portal.width, portal.height, portal.hBitmap, true);// ������
    ObjectCollision(object);

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

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����
    mciSendString(TEXT("play ..\\2Dtyaaaaaanki\\8-bitm.mp3 repeat"), NULL, 0, NULL);
    ShowCursor(NULL);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        ShowGame();//������ ���, ����� � �������
        ShowScore();//������ ���� ������
        BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
        Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)

        ProcessInput();//����� ����������
        LimitHero();//���������, ����� ������ ����� �� ������ �� ������� ������
        
    }

    clear();

}
