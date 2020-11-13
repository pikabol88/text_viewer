#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "components.h"
#include "scroll.h"
#include "textProcessing.h"
#include "textViewer.h"

#include "menu.h"

#include<stdio.h>
#include<conio.h>



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static char szAppName[] = _T("TextViewer");

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine,
                   int iCmdShow) {

    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = "menu";
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    RegisterClassEx(&wndclass);
    hwnd = CreateWindow( szAppName,
                         "Text Viewer",
                         WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         544,
                         375,
                         HWND_DESKTOP,
                         NULL,
                         hInstance,
                         (char*) szCmdLine
    );

    ShowWindow(hwnd, iCmdShow);

    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

/** \brief
 *  Заполнение информации о размере окне
 * \param RECT rect - структура прямоугольника
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void GetWindowSize(HWND hwnd, hViewer_t *hViewer, RECT rect) {
    GetClientRect(hwnd, &rect);
    hViewer->wndParam.size.horizontal = rect.right;
    hViewer->wndParam.size.vertical = rect.bottom;
}

/** \brief
 *  Заполнение OPENFILENAME структуры
 * \param PTCHAR name - имя файла
 * \param HWND hwnd - описатель окна
 * \return BOOL результат GetOpenFileName(&ofn)
 *  (Если пользователь указывает имя файла и нажимает кнопку ОК - TRUE)
 */
BOOL PopFileInitialize(HWND hwnd, PTCHAR name){

    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));
    ZeroMemory(name, sizeof(TCHAR)*MAX_PATH);
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = NULL;
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = TEXT("Please Select a File");
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = 0;
    ofn.lpfnHook = 0;
    ofn.lpTemplateName = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    return GetOpenFileName(&ofn);
}

/** \brief
 *  Освобождение памяти при смене режима просмотра текста
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 *
 */
void FreeLastData(hViewer_t *hViewer){
    if(hViewer->textInfo){
        if(hViewer->textInfo->shift){
            free(hViewer->textInfo->shift);
        }
        if(hViewer->textInfo->wrap){
            free(hViewer->textInfo->wrap);
            if(hViewer->textInfo->wrap->shift){
                free(hViewer->textInfo->wrap->shift);
            }
        }
        if(hViewer->textInfo->text){
            free(hViewer->textInfo->text);
        }
    }
}

/** \brief
 *  Начальное заполнение основной структуры для работы с просмотрщиком текста,
 *  стратовая инициализация полей структуры
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void StartInitialization(HWND hwnd, hViewer_t *hViewer, error_state_t *error){
    hViewer->mode = USUAL;
    FillTextInfo(hViewer->textInfo, error);
    FillWrapInfo(hViewer->textInfo, hViewer->wndParam.size.horizontal/hViewer->fontParam.charWidth,error);
    UploadScrollPosition(hViewer, hViewer->textInfo->wrap->shift[hViewer->scrollParam.currentPoint.vertical]);
    SetStartParam(hViewer);
    SetScrollParams(hwnd, hViewer);
}

/** \brief
 *  Проверка на то, произошла ли ошибка в ранее выхванной функции
 * \param error_state_t error - код ошибки
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void CheckSuccess(HWND hwnd, error_state_t error){
    switch(error){
        case SUCCESS:
            break;
        case FILE_ERROR:
            MessageBox(hwnd, "Sorry, the problem with reading file appeared during the program execution",
                                szAppName, MB_OK | MB_ICONERROR);
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            break;
        case MEMORY_ERROR:
            MessageBox(hwnd, "Sorry, the problem with memory allocation appeared during the program execution",
                                szAppName, MB_OK | MB_ICONERROR);
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            break;
    }

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

    static int iColorID[3] = { WHITE_BRUSH,GRAY_BRUSH, BLACK_BRUSH };

    static int iSelection = IDM_WHITE;

    HDC hdc;
    HMENU hMenu;

    static hViewer_t hViewer;
    static infoForScroll_t ifs;

    static TEXTMETRIC tm;
    static PAINTSTRUCT ps;
    static RECT rect;

    static error_state_t error = SUCCESS;

    static boolean textIsLoaded = TRUE;

    switch(iMsg) {
        case WM_COMMAND:
            hMenu = GetMenu(hwnd);
            switch(LOWORD(wParam)){

                case IDM_OPEN:{
                    char buffer[256];
                    if(PopFileInitialize(hwnd, buffer)){
                        FreeLastData(&hViewer);
                        MyReadFile((PCHAR)buffer,&hViewer, &error);
                        CheckSuccess(hwnd,error);
                        StartInitialization(hwnd, &hViewer,&error);
                        CheckSuccess(hwnd,error);
                        InvalidateRect(hwnd, 0, TRUE);
                        textIsLoaded = TRUE;
                    }
                    break;
                }

                case IDM_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0L);
                    return 0;

                case IDM_ABOUT:
                    MessageBox(hwnd,
                               "\tText Viewer Program\n\n\t\tcreated by:\n\t\tAlyona Voinova",
                                szAppName, MB_ICONINFORMATION | MB_OK);
                    return 0;

                case IDM_HELP:
                    MessageBox(hwnd, "This is a file viewer program. You can select the desired file and switch modes for viewing the file: with wrap and usual (without wrap)",
                            szAppName, MB_OK | MB_ICONQUESTION);
                    return 0;

                case IDM_WHITE :
                case IDM_GRAY :
                case IDM_BLACK :
                    CheckMenuItem(hMenu, iSelection, MF_UNCHECKED);
                    iSelection = LOWORD(wParam);
                    CheckMenuItem(hMenu, iSelection, MF_CHECKED);
                    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND,(LONG)GetStockObject(iColorID[LOWORD(wParam) - IDM_WHITE]));
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;

                case IDM_WRAP:
                    if(textIsLoaded){
                        CheckMenuItem(hMenu, IDM_WRAP, MF_CHECKED);
                        CheckMenuItem(hMenu, IDM_USUAL, MF_UNCHECKED);
                        EnableMenuItem(hMenu, IDM_WRAP, MF_DISABLED);
                        EnableMenuItem(hMenu, IDM_USUAL, MF_ENABLED);
                        if(!(hViewer.mode == WRAP)){
                            hViewer.mode = WRAP;
                            int currentPosition = hViewer.textInfo->shift[hViewer.scrollParam.currentPoint.vertical];

                            FillWrapInfo(hViewer.textInfo, hViewer.wndParam.size.horizontal/hViewer.fontParam.charWidth, &error);
                            CheckSuccess(hwnd,error);
                            FindScrollPosition(&hViewer, currentPosition);
                            //UploadScrollPosition(&hViewer, hViewer.textInfo->wrap->shift[hViewer.scrollParam.currentPoint.vertical]);
                            SetScrollParams(hwnd, &hViewer);

                            InvalidateRect(hwnd, 0, TRUE);
                        }
                    } else{
                        MessageBox(hwnd,
                               "\tOpen text file to start viewing the text",
                                szAppName, MB_ICONINFORMATION | MB_OK);
                    }
                    break;


                case IDM_USUAL:
                    if(textIsLoaded){
                        CheckMenuItem(hMenu, IDM_WRAP, MF_UNCHECKED);
                        CheckMenuItem(hMenu, IDM_USUAL, MF_CHECKED);
                        EnableMenuItem(hMenu, IDM_WRAP, MF_ENABLED);
                        EnableMenuItem(hMenu, IDM_USUAL, MF_DISABLED);
                        if(!hViewer.mode == USUAL){
                            hViewer.mode = USUAL;
                            int currentPosition = hViewer.textInfo->wrap->shift[hViewer.scrollParam.currentPoint.vertical];
                            FindScrollPosition(&hViewer, currentPosition);
                            free(hViewer.textInfo->shift);
                            hViewer.textInfo->shift = NULL;
                            CalculateShift(hViewer.textInfo, &error);
                            CheckSuccess(hwnd,error);
                            //UploadScrollPosition(&hViewer, hViewer.textInfo->wrap->shift[hViewer.scrollParam.currentPoint.vertical]);
                            SetScrollParams(hwnd, &hViewer);
                            InvalidateRect(hwnd, 0, TRUE);
                        }
                    }else{
                        MessageBox(hwnd,
                               "\tOpen text file to start viewing the text",
                                szAppName, MB_ICONINFORMATION | MB_OK);
                    }
                    break;
            }
            break;


        case WM_CREATE :

            hdc = GetDC(hwnd);
            GetTextMetrics(hdc, &tm);

            hViewer.mode = USUAL;
            FillFontInfo(&hViewer, tm);

            CREATESTRUCT *tmp = (CREATESTRUCT*)lParam;
            PCHAR param = (PCHAR)tmp->lpCreateParams;
            if(!strlen(param)){
                 MessageBox(hwnd,
                               "\tOpen text file to start viewing the text",
                                szAppName, MB_ICONINFORMATION | MB_OK);
                char buffer[256];
                if(PopFileInitialize(hwnd, buffer))
                    MyReadFile((PCHAR)buffer,&hViewer, &error);
                    CheckSuccess(hwnd,error);
                    textIsLoaded = TRUE;
            }else{
                MyReadFile(param, &hViewer, &error);
                CheckSuccess(hwnd,error);
            }
            GetWindowSize(hwnd, &hViewer, rect);
            FillTextInfo(hViewer.textInfo, &error);
            CheckSuccess(hwnd,error);
            FillWrapInfo(hViewer.textInfo, hViewer.wndParam.size.horizontal/hViewer.fontParam.charWidth, &error);
            CheckSuccess(hwnd,error);
            UploadScrollPosition(&hViewer, hViewer.textInfo->wrap->shift[hViewer.scrollParam.currentPoint.vertical]);
            SetStartParam(&hViewer);
            SetScrollParams(hwnd, &hViewer);
            ReleaseDC(hwnd, hdc);
            return 0;

        case WM_SIZE :
            GetWindowSize(hwnd,&hViewer, rect);
            if(textIsLoaded){
                UpdateWrapParams(&hViewer, &error);
                SetScrollParams(hwnd, &hViewer);
            }
            InvalidateRect(hwnd, 0, TRUE);
            return 0;

        case WM_PAINT :
            if(textIsLoaded){
                hdc = BeginPaint(hwnd, &ps);
                MyTextOut(hdc,&hViewer, ps);
                EndPaint(hwnd, &ps);
            }
            return 0;


        case WM_VSCROLL: {
            FillInfoForScroll(&ifs, &hViewer);
            switch ((int)LOWORD(wParam)) {

                case SB_LINEUP: ScrollLineUp(hwnd, &hViewer, ifs); break;
                case SB_LINEDOWN: ScrollLineDown(hwnd, &hViewer, ifs); break;
                case SB_PAGEUP:ScrollPageUp(hwnd, &hViewer, ifs); break;
                case SB_PAGEDOWN: ScrollPageDown(hwnd, &hViewer, ifs); break;

                case SB_THUMBPOSITION:
                    hViewer.scrollParam.position.vertical = HIWORD(wParam);
                    ScrollThumbVertical(hwnd, &hViewer, ifs);
                    break;
            }
            SetScrollPos(hwnd, SB_VERT, hViewer.scrollParam.position.vertical, TRUE);
            SetScrollPos(hwnd, SB_HORZ, hViewer.scrollParam.position.horizontal, TRUE);
            UpdateWindow(hwnd);
            break;
        }

        case WM_HSCROLL: {
            FillInfoForScroll(&ifs, &hViewer);
            switch ((int)LOWORD(wParam)) {

                case SB_LINERIGHT: ScrollLineRight(hwnd, &hViewer, ifs); break;
                case SB_LINELEFT:  ScrollLineLeft(hwnd, &hViewer, ifs); break;
                case SB_PAGERIGHT: ScrollPageRight(hwnd, &hViewer, ifs); break;
                case SB_PAGELEFT:  ScrollPageLeft(hwnd, &hViewer, ifs); break;

                case SB_THUMBPOSITION:
                    hViewer.scrollParam.position.horizontal = HIWORD(wParam);
                    ScrollThumbHorizontal(hwnd, &hViewer, ifs);
                    break;
            }
            SetScrollPos(hwnd, SB_VERT, hViewer.scrollParam.position.vertical, TRUE);
            SetScrollPos(hwnd, SB_HORZ, hViewer.scrollParam.position.horizontal, TRUE);
            break;
        }

        case WM_KEYDOWN: {
            FillInfoForScroll(&ifs, &hViewer);
            switch(wParam) {

                case VK_PRIOR: ScrollPageUp(hwnd, &hViewer, ifs); break;
                case VK_NEXT: ScrollPageDown(hwnd, &hViewer, ifs); break;

                case VK_UP: ScrollLineUp(hwnd, &hViewer, ifs); break;
                case VK_DOWN: ScrollLineDown(hwnd, &hViewer, ifs); break;
                case VK_RIGHT: ScrollLineRight(hwnd, &hViewer, ifs); break;
                case VK_LEFT: ScrollLineLeft(hwnd, &hViewer, ifs); break;
            }
            SetScrollPos(hwnd, SB_VERT, hViewer.scrollParam.position.vertical, TRUE);
            SetScrollPos(hwnd, SB_HORZ, hViewer.scrollParam.position.horizontal, TRUE);
            UpdateWindow(hwnd);
            break;

        }

        case WM_DESTROY :
            FreeLastData(&hViewer);
            if(hViewer.textInfo->wrap){
                free(hViewer.textInfo->wrap);
            }
            if(hViewer.textInfo){
                free(hViewer.textInfo);
            }
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
