#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <malloc.h>
#include "chain.h"

#include "stb-master/stb_easy_font.h"
#include "stb-master/stb_image.h"

#define nameLen 20

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void Menu_ShowMenu();


int width,height;

RECT rct;


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;


    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "The Mummy's Adventures";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "The Mummy's Adventures",
                          "The Mummy's Adventures",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          700,
                          500,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);


    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);


    GetClientRect(hwnd,&rct);




    init_Buttons_menu();


    while (!bQuit)
    {
        //check for messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //handle or dispatch messages
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //OpenGL animation code goes here
           glClearColor(0.0f, 0.5f, 0.5f, 0.0f);//очистка окна от предыдущего окна
           glClear(GL_COLOR_BUFFER_BIT);//очистка ...

           if(window==-1){
                glLoadIdentity();
                glOrtho(0, rct.right, rct.bottom, 0,  1, -1);
                Menu_ShowMenu();
           }
           if(window==1){
                glLoadIdentity();
                glOrtho(0, rct.right, rct.bottom, 0,  1, -1);
                Menu_ShowMenu();
                glLoadIdentity();
                glOrtho(-50, rct.right,  -50, rct.bottom,  1, -1);
                ShowBackround(background);
           }


           SwapBuffers(hDC);//смена кадра
           Sleep (1);
        }
    }


    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
    break;

    case WM_MOUSEMOVE:
        Menu_MouseMove(LOWORD(lParam),HIWORD(lParam));
    break;

    case WM_LBUTTONUP:
        Menu_MouseUp();
    break;

    case WM_LBUTTONDOWN:
        Menu_MouseDown();
        MouseDown();



    break;


    case WM_SIZE:

       /* width=LOWORD(lParam);
        height=HIWORD(lParam);
        glViewport(0,0,width,height);
       // glLoadIdentity();
        float p;
        p=width/(float)height;
        glOrtho(0, p, -p, 0, 1, -1);
        //glOrtho(-p,p,-1,1, -1,1);*/
    break;

    case WM_DESTROY:
        return 0;
    break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


void Menu_ShowMenu()
{

    glPushMatrix();
        //glLoadIdentity();//сбрасывает матрицу обратно в состояние по умолчанию
        //glOrtho(-1,1,1,-1,0,1);//создает "Наклонную" проекцию,ось влевом верхнем углу экрана
        for(int i=0; i<btnCnt; i++)   ShowButton(i);
    glPopMatrix();
}
