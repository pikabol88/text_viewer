#ifndef SCROLL_H
#define SCROLL_H
#include <windows.h>
#include "components.h"

#define MAX_SCROLL_RANGE 65534
//#define max(a, b) (((a) > (b)) ? (a) : (b))
//#define min(a, b) (((a) < (b)) ? (a) : (b))

void SetScrollParams(HWND hwnd, hViewer_t *hViewer);
void SetStartParam(hViewer_t *hViewer);
void FillInfoForScroll(infoForScroll_t *ifs,   hViewer_t *hViewer);
void ScrollLineDown(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollLineUp(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollLineRight(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollLineLeft(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);

void ScrollPageRight(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollPageLeft(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollPageDown(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollPageUp(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);

void ScrollThumbVertical(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void ScrollThumbHorizontal(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs);
void UploadScrollPosition(hViewer_t  *hViewer, int currentPosition);

void FindScrollPosition(hViewer_t *hViewer, int currentPosition);

#endif //SCROLL_H
