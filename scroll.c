#include "scroll.h"

/** \brief
 *  Заполнение структуры для упрощения работы со скрулом
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 *
 */
void FillInfoForScroll(infoForScroll_t *ifs, hViewer_t *hViewer){
    ifs->currentLine = hViewer->scrollParam.currentPoint.vertical;
    ifs->numOfLinesInWindow = hViewer->wndParam.size.vertical / hViewer->fontParam.charHeight;
    ifs->numOfLinesInText = hViewer->mode == USUAL ? hViewer->textInfo->numOfLines : hViewer->textInfo->wrap->numOfLines;
    ifs->currentSymbol = hViewer->scrollParam.currentPoint.horizontal;
    ifs->maxSymbolsInLine = hViewer->wndParam.size.horizontal / hViewer->fontParam.charWidth;
}

/** \brief
 *  Заполнение информации для скрула
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 *
 */
void SetStartParam(hViewer_t *hViewer) {
    hViewer->scrollParam.currentPoint.horizontal = 0;
    hViewer->scrollParam.currentPoint.vertical = 0;
    hViewer->scrollParam.position.vertical = 0;
    hViewer->scrollParam.position.horizontal = 0;
}

/** \brief
 *  Установка коэффициентов скрулла
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 *
 */
void SetScrollCoefficient(infoForScroll_t *ifs, hViewer_t *hViewer){
    if (max(0, min(ifs->numOfLinesInText - ifs->numOfLinesInWindow, MAX_SCROLL_RANGE)) < MAX_SCROLL_RANGE) {
        hViewer->scrollParam.coefficient.vertical = 1;
    } else {
        hViewer->scrollParam.coefficient.vertical =
                (float) (ifs->numOfLinesInText - ifs->numOfLinesInWindow) / (float) MAX_SCROLL_RANGE;
    }
    if (max(0, min((int)hViewer->textInfo->maxLength - ifs->maxSymbolsInLine, MAX_SCROLL_RANGE)) < MAX_SCROLL_RANGE) {
        hViewer->scrollParam.coefficient.horizontal = 1;
    } else {
        hViewer->scrollParam.coefficient.horizontal =
                (float) (hViewer->textInfo->maxLength - ifs->maxSymbolsInLine) / (float) MAX_SCROLL_RANGE;
    }
}

/** \brief
 *  Перерасчет вертикального скрулла и блокировка скрулла в случае, если он не нужен
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd- описатель окна
 * \return void
 *
 */
void CheckVerticalScroll(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.numOfLinesInText -ifs.numOfLinesInWindow - ifs.currentLine < 0) {
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
        if (hViewer->scrollParam.position.vertical - (int)hViewer->scrollParam.coefficient.vertical >= 0) {
            hViewer->scrollParam.currentPoint.vertical--;
            hViewer->scrollParam.position.vertical -= (int)hViewer->scrollParam.coefficient.vertical;
        }
    } else {
        if (ifs.numOfLinesInText -ifs.numOfLinesInWindow == 0){
            EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
        } else{
            EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);
        }
    }
}


/** \brief
 *  Перерасчет горизонтального скрулла и блокировка скрулла в случае, если он не нужен
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void CheckHorizontalScroll(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if ((int)hViewer->textInfo->maxLength - ifs.maxSymbolsInLine - ifs.currentSymbol < 0 || hViewer->mode == WRAP) {
        EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
        ShowScrollBar(hwnd, SB_HORZ, FALSE);
        if (hViewer->scrollParam.position.horizontal - (int)hViewer->scrollParam.coefficient.horizontal >= 0) {
            hViewer->scrollParam.currentPoint.horizontal--;
            hViewer->scrollParam.position.horizontal -= (int)hViewer->scrollParam.coefficient.horizontal;
        }
    } else {
        if (hViewer->textInfo->maxLength -  ifs.maxSymbolsInLine == 0){
            EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
            ShowScrollBar(hwnd, SB_HORZ, FALSE);
        } else {
            EnableScrollBar(hwnd, SB_HORZ, ESB_ENABLE_BOTH);
            ShowScrollBar(hwnd, SB_HORZ, TRUE);
        }
    }
}

/** \brief
 *  Заполнение основной структуры для работы с просмотрщиком текста информацией о скрулле
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void SetScrollParams(HWND hwnd, hViewer_t * hViewer) {
    infoForScroll_t  ifs;
    FillInfoForScroll(&ifs, hViewer);
    SetScrollCoefficient(&ifs, hViewer);
    hViewer->scrollParam.position.vertical = (int)hViewer->scrollParam.coefficient.vertical * hViewer->scrollParam.currentPoint.vertical;

    CheckVerticalScroll(hwnd, hViewer, ifs);
    CheckHorizontalScroll(hwnd, hViewer, ifs);

    SetScrollRange(hwnd, SB_VERT, 0, min(ifs.numOfLinesInText -ifs.numOfLinesInWindow ,MAX_SCROLL_RANGE), TRUE);
    SetScrollRange(hwnd, SB_HORZ, 0, min(hViewer->textInfo->maxLength - ifs.maxSymbolsInLine, MAX_SCROLL_RANGE), TRUE);
    SetScrollPos(hwnd, SB_VERT, hViewer->scrollParam.position.vertical, TRUE);
    SetScrollPos(hwnd, SB_HORZ, hViewer->scrollParam.position.horizontal, TRUE);


}

/** \brief
 * Обновление текущего положения скрулла
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param int currentPosition - ??????? ??????? ????????????? ???????
 * \return void
 *
 */
void UploadScrollPosition(hViewer_t  *hViewer, int currentPosition) {
    if (hViewer->mode == WRAP){
        for (int i = 0; i < hViewer->textInfo->wrap->numOfLines; i++) {
            if (currentPosition >= hViewer->textInfo->wrap->shift[i] && currentPosition < hViewer->textInfo->wrap->shift[i + 1]) {
                hViewer->scrollParam.currentPoint.vertical = i;
                break;
            }
        }
    }
}

/** \brief
 *  Перемещение на строку(или более,в случае больших текстов) вниз
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollLineDown(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentLine + ifs.numOfLinesInWindow < ifs.numOfLinesInText) {
        hViewer->scrollParam.currentPoint.vertical++;
        hViewer->scrollParam.position.vertical += (int)hViewer->scrollParam.coefficient.vertical;
        ScrollWindow(hwnd, 0, -hViewer->fontParam.charHeight, NULL, NULL);
        InvalidateRect(hwnd, 0, TRUE);
    }
}

/** \brief
 *  Перемещение на строку(или более,в случае больших текстов) вверх
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollLineUp(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentLine) {
        hViewer->scrollParam.currentPoint.vertical--;
        hViewer->scrollParam.position.vertical-= (int)hViewer->scrollParam.coefficient.vertical;
        ScrollWindow(hwnd, 0, hViewer->fontParam.charHeight, NULL, NULL);
        InvalidateRect(hwnd, 0, TRUE);
    }
}

/** \brief
 *  Перемещение на строку(или более,в случае больших текстов) вправо
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollLineRight(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentSymbol + ifs.maxSymbolsInLine < hViewer->textInfo->maxLength) {
        hViewer->scrollParam.currentPoint.horizontal++;
        hViewer->scrollParam.position.horizontal += (int)hViewer->scrollParam.coefficient.horizontal;
        InvalidateRect(hwnd, 0, TRUE);
    }
}

/** \brief
 *  Перемещение на строку(или более,в случае больших текстов) влево
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollLineLeft(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentSymbol > 0) {
        hViewer->scrollParam.currentPoint.horizontal--;
        hViewer->scrollParam.position.horizontal -= (int)hViewer->scrollParam.coefficient.horizontal;
        InvalidateRect(hwnd, 0, TRUE);
    }
}

/** \brief
 *  Перемещение на страницу(или более,в случае больших текстов) вверх
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollPageUp(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentLine >  ifs.numOfLinesInWindow) {
        hViewer->scrollParam.currentPoint.vertical -=  ifs.numOfLinesInWindow;
        hViewer->scrollParam.position.vertical -= (int)hViewer->scrollParam.coefficient.vertical * ifs.numOfLinesInWindow;
    } else {
        hViewer->scrollParam.currentPoint.vertical = 0;
        hViewer->scrollParam.position.vertical = 0;
    }
    InvalidateRect(hwnd,    0, TRUE);
}

/** \brief
 *  Перемещение на страницу(или более,в случае больших текстов) вниз
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollPageDown(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if ((ifs.currentLine + 2 * ifs.numOfLinesInWindow) < hViewer->textInfo->numOfLines) {
        hViewer->scrollParam.currentPoint.vertical += ifs.numOfLinesInWindow;
        hViewer->scrollParam.position.vertical += (int)hViewer->scrollParam.coefficient.vertical * ifs.numOfLinesInWindow;
    } else {
        hViewer->scrollParam.currentPoint.vertical = hViewer->textInfo->numOfLines - ifs.numOfLinesInWindow;
        hViewer->scrollParam.position.vertical = (int)hViewer->scrollParam.coefficient.vertical * (hViewer->textInfo->numOfLines - ifs.numOfLinesInWindow);
    }
    InvalidateRect(hwnd,    0, TRUE);
}

/** \brief
 *  Перемещение на страницу(или более,в случае больших текстов) вправо
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollPageRight(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if ((ifs.currentSymbol + 2 * ifs.maxSymbolsInLine) < hViewer->textInfo->maxLength) {
        hViewer->scrollParam.currentPoint.horizontal += ifs.maxSymbolsInLine;
        hViewer->scrollParam.position.horizontal += ((int)hViewer->scrollParam.coefficient.horizontal * ifs.maxSymbolsInLine);
    } else {
        hViewer->scrollParam.currentPoint.horizontal = hViewer->textInfo->maxLength - ifs.maxSymbolsInLine;
        hViewer->scrollParam.position.horizontal= (int)hViewer->scrollParam.coefficient.horizontal * (hViewer->textInfo->maxLength - ifs.maxSymbolsInLine);
    }
    InvalidateRect(hwnd, 0, TRUE);
}

/** \brief
 *  Перемещение на страницу(или более,в случае больших текстов) влево
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollPageLeft(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    if (ifs.currentSymbol > ifs.maxSymbolsInLine) {
        hViewer->scrollParam.currentPoint.horizontal -= ifs.maxSymbolsInLine;
        hViewer->scrollParam.position.horizontal -= ((int)hViewer->scrollParam.coefficient.horizontal * ifs.maxSymbolsInLine);
    } else {
        hViewer->scrollParam.currentPoint.horizontal = 0;
        hViewer->scrollParam.position.horizontal = 0;
    }
    InvalidateRect(hwnd,    0, TRUE);
}

/** \brief
 *  Перемещение горизонтальной позиции скрулла при перетаскивании полосы прокрутки пользователем
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollThumbHorizontal(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    hViewer->scrollParam.currentPoint.horizontal = hViewer->scrollParam.position.horizontal * hViewer->scrollParam.coefficient.horizontal;
    if (hViewer->scrollParam.currentPoint.horizontal == hViewer->textInfo->maxLength)  {
        hViewer->scrollParam.currentPoint.horizontal -= ifs.maxSymbolsInLine;
    }
    InvalidateRect(hwnd, 0, TRUE);
}

/** \brief
 *  Перемещение вертикальной позиции скрулла при перетаскивании полосы прокрутки пользователем
 * \param infoForScroll_t *ifs - структура с информацией для работы со скруллом
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param HWND hwnd - описатель окна
 * \return void
 *
 */
void ScrollThumbVertical(HWND hwnd, hViewer_t *hViewer, infoForScroll_t ifs){
    hViewer->scrollParam.currentPoint.vertical= hViewer->scrollParam.position.vertical * hViewer->scrollParam.coefficient.vertical;
    if (hViewer->scrollParam.currentPoint.vertical == hViewer->textInfo->numOfLines) {
        hViewer->scrollParam.currentPoint.vertical -= ifs.numOfLinesInWindow;
    }
    InvalidateRect(hwnd,    0, TRUE);
}

/** \brief
 *  Перерасчет текущей позиции скрулла при переключении режимов просмотра текста с версткой
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param int currentPosition  - текущая позиция ползунка скрулла
 * \return void
 * \return void
 *
 */
void FindScrollPositionWrap(hViewer_t *hViewer, int currentPosition){
    for (int i = 0; i < hViewer->textInfo->wrap->numOfLines; i++) {
        if (currentPosition >= hViewer->textInfo->wrap->shift[i] && currentPosition < hViewer->textInfo->wrap->shift[i + 1]) {
            hViewer->scrollParam.currentPoint.vertical = i;
            break;
        }
    }
}

/** \brief
 *  Перерасчет текущей позиции скрулла при переключении на обычный режим просмотра текста
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param int currentPosition  - текущая позиция ползунка скрулла
 * \return void
 * \return void
 *
 */
void FindScrollPositionUsual(hViewer_t *hViewer, int currentPosition){
    for (int i = 0; i < hViewer->textInfo->numOfLines; i++) {
        if (currentPosition >= hViewer->textInfo->shift[i] && currentPosition < hViewer->textInfo->shift[i + 1]) {
            hViewer->scrollParam.currentPoint.vertical = i;
            break;
        }
    }
}

/** \brief
 *  Перерасчет текущей позиции скрулла при переключении режимов просмотра текста
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param int currentPosition - текущая позиция ползунка скрулла
 * \return void
 *
 */
void FindScrollPosition(hViewer_t *hViewer, int currentPosition) {
    if (hViewer->mode == WRAP) {
        FindScrollPositionWrap(hViewer, currentPosition);
    }
    else if (hViewer->mode == USUAL){
        FindScrollPositionUsual(hViewer, currentPosition);
    }
}
