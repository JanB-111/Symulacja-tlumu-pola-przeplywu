#include <Windowsx.h>
#include "framework.h"
#include "FlowFields.h"
#include "Symulacja.h"
#include <iostream>
#include <time.h>
#include <string>
#include "Resource.h" 
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <crtdbg.h>

#ifdef LOG_CONSOLE
#include "guicon.h"
#endif

#ifndef _USE_OLD_OSTREAMS
using namespace std;
#endif



bool przeprowadzanie_symulacji = false;
bool pokaz_pole = true;
bool need_resize = false;
//Parametry
float xPos;
float yPos;

int xPozycjaMysz;
int yPozycjaMysz;

float rozmiarKwadratu;
float predkosc = 0;

int wysokoscOkna=400;
int szerokoscOkna=400;

//rozmiar siatki
int wiersze=10;
int kolumny=10;

int liczba_aktorow=10;

bool cel_ustawiony = false;

bool stworzonoAktorow = false;

Symulacja symulacja;


#define MAX_LOADSTRING 100


HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

#ifdef LOG_CONSOLE
    RedirectIOToConsole();
#endif

    srand(time(NULL));

    symulacja.stworzSiatke(wiersze, kolumny);


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FLOWFIELDS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLOWFIELDS));

    MSG msg;

    // Główna pętla komunikatów:
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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLOWFIELDS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FLOWFIELDS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
       

   wcex.hbrBackground = NULL;

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

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



BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {

        case IDC_SYMULACJA:

            przeprowadzanie_symulacji = !przeprowadzanie_symulacji;
            break;

        case IDC_STWORZ_AKTOROW:
        {
            HWND liczba_aktorow_uchwyt = GetDlgItem(hwnd, IDC_LICZBA_AKTOROW);
            int dlugosc = GetWindowTextLength(liczba_aktorow_uchwyt);
            WCHAR buffor[50];

            GetWindowText(liczba_aktorow_uchwyt, buffor, dlugosc + 1);
            wstring temp = buffor;

            if (temp != L"") {
                int temp_l_aktorow = stoi(temp);

                if ((temp_l_aktorow <= 10000) && (temp_l_aktorow > 0)) {
                    liczba_aktorow = temp_l_aktorow;
                }
            }

            symulacja.delete_actors();
            symulacja.set_liczba_aktorow(liczba_aktorow);
            symulacja.set_kolumny(kolumny);
            symulacja.set_wiersze(wiersze);
            symulacja.generate_actors(kolumny, wiersze);
            stworzonoAktorow = true;
        }
        break;

        case IDC_WIDOCZNOSC_POLA:
            pokaz_pole = !pokaz_pole;
            break;

        case IDC_LOSUJ:
        {

            symulacja.losujPrzeszkody();
            Siatka* grid = symulacja.zwrocSiatke();
            if (cel_ustawiony == true) {
                grid->oblicz_pole_integracji();
                grid->oblicz_kierunki();
            }
        }
            break;

        case IDC_SIATKA:
            stworzonoAktorow = false;
            symulacja.zniszcz_siatke();

            HWND rozmiar_siatki = GetDlgItem(hwnd, IDC_ROZMIAR_SIATKI);
            int rozmiar_temp = GetWindowTextLength(rozmiar_siatki);
            WCHAR buffor_temp[50];
            GetWindowText(rozmiar_siatki, buffor_temp, rozmiar_temp + 1);
            wstring temp_siatka = buffor_temp;

            if (temp_siatka != L"") {
                int temp_rozmiar_siatki = stoi(temp_siatka);

                if ((temp_rozmiar_siatki <= 75) && (temp_rozmiar_siatki > 2)) {
                    wiersze = temp_rozmiar_siatki;
                    kolumny = temp_rozmiar_siatki;
                }
            }

            symulacja.stworzSiatke(wiersze, kolumny);
            need_resize = true;
            cel_ustawiony = false;

            break;




        }

    default:
        return FALSE;
    }
    return TRUE;
}


 HWND g_handle = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,  LPARAM lParam)
{

    static HDC hdc=NULL;
    static HBITMAP hBmp=NULL;
    static HDC hMemDC=NULL;


    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {

        g_handle = CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_FLOW_FIELDS_INPUT),hWnd,ToolDlgProc);
        if (g_handle != NULL)
        {
            ShowWindow(g_handle, SW_SHOW);
        }
        else
        {
            MessageBox(hWnd, L"Funkcja CreateDialog zwróciła NULL", L"Nieudana inicjalizacja okna ustawień!",
                MB_OK | MB_ICONINFORMATION);
        }
        
        SetTimer(hWnd, 1001, 105, NULL);
        hBmp = CreateBitmap( szerokoscOkna, wysokoscOkna,1,24,NULL);

    }
    break;

    case WM_TIMER:
    {
        if (stworzonoAktorow) {

            if (przeprowadzanie_symulacji == true) {
                symulacja.nastepny_krok(liczba_aktorow, predkosc);
            }
            
        }

        InvalidateRect(hWnd, NULL, true);
        return 0;
    }
    break;

    case WM_PAINT:
    {

      if (need_resize == true) {
            float rozmiarKwadratu_x_temp = szerokoscOkna / kolumny;
            float rozmiarKwadratu_y_temp = wysokoscOkna / wiersze;

            if (rozmiarKwadratu_x_temp < rozmiarKwadratu_y_temp) {
                rozmiarKwadratu = szerokoscOkna / kolumny;
            }
            else {
                rozmiarKwadratu = wysokoscOkna / wiersze;
            }

            symulacja.set_rozmiar_wyswietlany(rozmiarKwadratu);
            predkosc = rozmiarKwadratu / 10;
            need_resize = false;
        }

        RECT Client_Rect;
        GetClientRect(hWnd, &Client_Rect);
        int win_width = Client_Rect.right - Client_Rect.left;
        int win_height = Client_Rect.bottom + Client_Rect.left;
        PAINTSTRUCT ps;
        HDC Memhdc;
        HDC hdc;
        HBITMAP Membitmap;

        hdc = BeginPaint(hWnd, &ps);
        Memhdc = CreateCompatibleDC(hdc);
        Membitmap = CreateCompatibleBitmap(hdc, win_width, win_height);

        SelectObject(Memhdc, Membitmap);

        Siatka* grid = symulacja.zwrocSiatke();
        LONG L_Wiersze = 0, L_Kolumny = 0;

        HBRUSH zielony = CreateSolidBrush(RGB(150, 250, 150));
        HBRUSH jasno_zielony = CreateSolidBrush(RGB(10, 100, 10));
        HBRUSH ciemno_zielony = CreateSolidBrush(RGB(0, 20, 0));

        for (int i = 0; i < wiersze; i++) {
            L_Wiersze = i * rozmiarKwadratu;
            for (int j = 0; j < kolumny; j++) {
                L_Kolumny = j * rozmiarKwadratu;

                if (grid->podajKosztDlaPola(i, j) == 0) {
                    tagRECT x{ L_Kolumny, L_Wiersze, L_Kolumny + rozmiarKwadratu, L_Wiersze + rozmiarKwadratu };
                    SelectObject(Memhdc, zielony);
                    FillRect(Memhdc, &x, zielony);
                }
                else if (grid->podajKosztDlaPola(i, j) == 4) {
                    tagRECT x{ L_Kolumny, L_Wiersze, L_Kolumny + rozmiarKwadratu, L_Wiersze + rozmiarKwadratu };
                    SelectObject(Memhdc, jasno_zielony);

                    FillRect(Memhdc, &x, jasno_zielony);
                }
                else if (grid->podajKosztDlaPola(i, j) == 255) {
                    tagRECT x{ L_Kolumny, L_Wiersze, L_Kolumny + rozmiarKwadratu, L_Wiersze + rozmiarKwadratu };
                    SelectObject(Memhdc, ciemno_zielony);
                    FillRect(Memhdc, &x, ciemno_zielony);
                }
            }
        }

        if(pokaz_pole==true){
            for (int i = 0; i < wiersze; i++) {
                L_Wiersze = i * rozmiarKwadratu;
                for (int j = 0; j < kolumny; j++) {
                    L_Kolumny = j * rozmiarKwadratu;

                    SelectObject(ps.hdc, GetStockObject(BLACK_PEN));

                    if (grid->podajKierunki(i, j) == N) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze);
                    }
                    else if (grid->podajKierunki(i, j) == NW) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny, L_Wiersze);
                    }
                    else if (grid->podajKierunki(i, j) == NE) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny + rozmiarKwadratu, L_Wiersze);
                    }
                    else if (grid->podajKierunki(i, j) == S) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu);
                    }
                    else  if (grid->podajKierunki(i, j) == SE) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny + rozmiarKwadratu, L_Wiersze + rozmiarKwadratu);
                    }
                    else  if (grid->podajKierunki(i, j) == SW) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny, L_Wiersze + rozmiarKwadratu);
                    }
                    else  if (grid->podajKierunki(i, j) == W) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny, L_Wiersze + rozmiarKwadratu / 2);
                    }
                    else  if (grid->podajKierunki(i, j) == E) {
                        MoveToEx(Memhdc, L_Kolumny + rozmiarKwadratu / 2, L_Wiersze + rozmiarKwadratu / 2, NULL);
                        LineTo(Memhdc, L_Kolumny + rozmiarKwadratu, L_Wiersze + rozmiarKwadratu / 2);
                    }
                }
            }
        }

        if (stworzonoAktorow == true) {
            for (int cykl = 0; cykl < liczba_aktorow; cykl++) {
                Aktor* test = grid->zwrotAktora(cykl);
                float tempL = rozmiarKwadratu / 6;

                SelectObject(Memhdc, GetStockObject(GRAY_BRUSH));

                Ellipse(Memhdc, (test->get_pos_kolumna() * rozmiarKwadratu) - tempL, (test->get_pos_wiersz() * rozmiarKwadratu) - tempL, (test->get_pos_kolumna() * rozmiarKwadratu) + tempL, (test->get_pos_wiersz() * rozmiarKwadratu) + tempL);
            }
        }

        if ((grid->get_cel().get_wiersz() != (-1)) && (grid->get_cel().get_kolumna() != (-1))) {

            HPEN cel = CreatePen(PS_DASH, 1, RGB(250, 0, 0));
            HBRUSH czerwien = CreateSolidBrush(RGB(100, 20, 20));

            int temp_y = grid->get_cel().get_wiersz();
            int temp_x = grid->get_cel().get_kolumna();

            SelectObject(Memhdc, czerwien);

            Ellipse(Memhdc, temp_x * rozmiarKwadratu, temp_y * rozmiarKwadratu, (temp_x + 1) * rozmiarKwadratu, (temp_y + 1) * rozmiarKwadratu);
        }

        BitBlt(hdc, 0, 0, win_width, win_height, Memhdc, 0, 0, SRCCOPY);
        
        DeleteObject(Membitmap);
        DeleteDC(Memhdc);
        DeleteDC(hdc);
        DeleteDC(hMemDC);
        DeleteObject(zielony);
        DeleteObject(ciemno_zielony);
        DeleteObject(jasno_zielony);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_MOUSEMOVE:
    {
        yPos = GET_X_LPARAM(lParam);
        xPos = GET_Y_LPARAM(lParam);
    }
    break;

    case WM_SIZE:
    {
        szerokoscOkna = LOWORD(lParam);
        wysokoscOkna = HIWORD(lParam);

        float rozmiarKwadratu_x_temp = szerokoscOkna / kolumny;
        float rozmiarKwadratu_y_temp = wysokoscOkna / wiersze;

        if (rozmiarKwadratu_x_temp < rozmiarKwadratu_y_temp) {
            rozmiarKwadratu = szerokoscOkna / kolumny;
        }
        else {
            rozmiarKwadratu = wysokoscOkna / wiersze;
        }

        symulacja.set_rozmiar_wyswietlany(rozmiarKwadratu);
        predkosc = rozmiarKwadratu / 10;

    }
    break;

    case WM_LBUTTONDOWN:
    {
        int temp_y = yPos / rozmiarKwadratu;
        int temp_x = xPos / rozmiarKwadratu;

        Siatka* grid = symulacja.zwrocSiatke();

        if ((temp_y < wiersze) && (temp_x < kolumny)) {

            Siatka* grid = symulacja.zwrocSiatke();

            if (grid->podajKosztDlaPola(temp_x, temp_y) == 0) {
                grid->setKoszt(temp_x, temp_y, 4);
            }
            else if (grid->podajKosztDlaPola(temp_x, temp_y) == 4) {
                grid->setKoszt(temp_x, temp_y, 255);
            }
            else {
                grid->setKoszt(temp_x, temp_y, 0);
            }

        }
        if (cel_ustawiony ==true) {
            grid->oblicz_pole_integracji();
            grid->oblicz_kierunki();
        }

        InvalidateRect(NULL, NULL, NULL);
    }
    break;

    case WM_RBUTTONDOWN:
    {

        int temp_y = yPos / rozmiarKwadratu;
        int temp_x = xPos / rozmiarKwadratu;

        if ((temp_y < wiersze) && (temp_x < kolumny)) {

            Siatka* grid = symulacja.zwrocSiatke();

            grid->set_cel(temp_x, temp_y);

            cel_ustawiony = true;

            grid->oblicz_pole_integracji();
            grid->oblicz_kierunki();

            Lokacja temp = grid->get_cel();

        }
        InvalidateRect(NULL, NULL, NULL);
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



INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
