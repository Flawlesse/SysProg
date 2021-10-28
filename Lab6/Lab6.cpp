// Lab6.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab6.h"
#include <ctime>

#define MAX_LOADSTRING 100

#define IDB_START 1
#define IDB_STOP 2

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

DWORD THREAD_ID_1;
DWORD THREAD_ID_2;
DWORD THREAD_ID_3;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD   CALLBACK    moveNote(LPVOID*);

enum DIRECTION { LEFT, UP, RIGHT, DOWN };

typedef struct _note {
  int x;
  int y;
  LPTSTR str;
}Note;

typedef struct _params {
    HWND hWnd;
    unsigned int speed;
    DIRECTION dir;
    Note note;
}PARAMS;


Note txt1, txt2, txt3;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hStart;
    static HWND hStop;

    static HANDLE hThread1;
    static HANDLE hThread2;
    static HANDLE hThread3;

    switch (message)
    {
    case WM_CREATE:
    {
        hStart = CreateWindow(L"button", L"Start",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            100, 100, 200, 50, hWnd, (HMENU)IDB_START, hInst, NULL);
        hStop = CreateWindow(L"button", L"Stop",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            700, 100, 200, 50, hWnd, (HMENU)IDB_STOP, hInst, NULL);

        RECT rc;
        GetClientRect(hWnd, &rc);
        txt1 = { rc.left + 100, rc.top + 300, LPTSTR(L"SAY HELLO TO MA LIL FRIEND!") };
        txt2 = { rc.left + 100, rc.top + 360, LPTSTR(L"halloow...") };
        txt3 = { rc.left + 100, rc.top + 390, LPTSTR(L"Spaceglidin' to the Moon") };

        PARAMS *p1 = new PARAMS{ hWnd, static_cast<unsigned int>(100), DIRECTION::LEFT, txt1 };
        PARAMS *p2 = new PARAMS{ hWnd, static_cast<unsigned int>(200), DIRECTION::RIGHT, txt2 };
        PARAMS *p3 = new PARAMS{ hWnd, static_cast<unsigned int>(300), DIRECTION::UP, txt3 };



        hThread1 = CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)moveNote, (LPVOID*)p1, CREATE_SUSPENDED, (LPDWORD)&THREAD_ID_1);
        hThread2 = CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)moveNote, (LPVOID*)p2, CREATE_SUSPENDED, (LPDWORD)&THREAD_ID_2);
        hThread3 = CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)moveNote, (LPVOID*)p3, CREATE_SUSPENDED, (LPDWORD)&THREAD_ID_3);
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDB_START:
                ResumeThread(hThread1);
                ResumeThread(hThread2);
                ResumeThread(hThread3);
                break;
            case IDB_STOP:
                SuspendThread(hThread1);
                SuspendThread(hThread2);
                SuspendThread(hThread3);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            //TextOut(hdc, txt1.x, txt1.y, txt1.str, _tcsclen(txt1.str));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

DWORD CALLBACK moveNote(LPVOID* data) {
    static const auto tickrate = 500;
    PARAMS* pr = (PARAMS*)data;
    HWND hWnd = pr->hWnd;

    Note note = pr->note;
    unsigned int sleepover = tickrate - pr->speed;
    DIRECTION dir = pr->dir;

    while (TRUE) {
        RECT rc;  
        PAINTSTRUCT ps;
        GetClientRect(hWnd, &rc);
        HDC hdc = GetWindowDC(hWnd);

        SelectObject(hdc, GetStockObject(DC_PEN));
        SetDCPenColor(hdc, RGB(255, 255, 255));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(255, 255, 255));
        Rectangle(hdc, note.x, note.y, note.x + _tcsclen(note.str) * 16, note.y + 16);

        switch (dir) {
        case DIRECTION::LEFT:
          if (note.x <= rc.left) {
            note.x = rc.right;
          }
          else {
            note.x -= 10;
          }
          break;
        case DIRECTION::UP:
          if (note.y <= rc.top) {
            note.y = rc.bottom;
          }
          else {
            note.y -= 10;
          }
          break;
        case DIRECTION::RIGHT:
          if (note.x > rc.right) {
            note.x = rc.left;
          }
          else {
            note.x += 10;
          }
          break;
        case DIRECTION::DOWN : 
          if (note.y > rc.bottom) {
            note.y = rc.top;
          }
          else {
            note.y += 10;
          }
          break;
        default:
          break;
        }
        
        
        TextOut(hdc, note.x, note.y, note.str, _tcsclen(note.str));
        InvalidateRect(hWnd, &rc, FALSE);
        Sleep(sleepover);
    }

    delete pr;
    return 0;
}