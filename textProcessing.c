#include "textProcessing.h"

/** \brief
 *  ����� ������ � ���� � ������ � ��������
 * \param HDC hdc - ��������� ��������� ���������
 * \param hViewer_t *hViewer - �������� ��������� ��� ������ � ������������� ������
 * \param PAINTSTRUCT ps - ��������� ���������
 * \return void
 */
void WrapTextOut(HDC hdc, hViewer_t *hViewer, PAINTSTRUCT ps) {
    int curString = hViewer->scrollParam.currentPoint.vertical;
    int countNum;
    char *textForOut;

    int paintBegin = ps.rcPaint.top/hViewer->fontParam.charHeight - 1;
    if(paintBegin<0){
        paintBegin = 0;
    }
    int paintEnd = min(ps.rcPaint.bottom/hViewer->fontParam.charHeight,
                       min(hViewer->wndParam.size.vertical/hViewer->fontParam.charHeight,
                           hViewer->textInfo->wrap->numOfLines - 1 - curString)) + 1;

	int i = paintBegin;
    for (i = paintBegin; i < (hViewer->textInfo->wrap->numOfLines - curString) && i < paintEnd; i++) {
        countNum =  ( hViewer->textInfo->wrap->shift[curString + i + 1] -  hViewer->textInfo->wrap->shift[curString + i]);
        textForOut =  hViewer->textInfo->text + hViewer->textInfo->wrap->shift[curString + i];
        TextOut(hdc, 0, i * hViewer->fontParam.charHeight,textForOut, countNum - 1);
    }
}

/** \brief
 *  ����� ������ � ���� � ������ ��� �������
 * \param HDC hdc - ��������� ��������� ���������
 * \param hViewer_t *hViewer - �������� ��������� ��� ������ � ������������� ������
 * \param PAINTSTRUCT ps - ��������� ���������
 * \return void
 */
void UsualTextOut(HDC hdc, hViewer_t *hViewer, PAINTSTRUCT ps) {
    int currentLine =  hViewer->scrollParam.currentPoint.vertical;
    int curLineLength;
    int countNum;
    char *textForOut;

    int paintBegin = ps.rcPaint.top/hViewer->fontParam.charHeight;
    int paintEnd = min(ps.rcPaint.bottom/hViewer->fontParam.charHeight,
                       min(hViewer->wndParam.size.vertical / hViewer->fontParam.charHeight,
                           hViewer->textInfo->numOfLines - 1 - currentLine)) + 1;

    int i = paintBegin;
    for (i = paintBegin; i < paintEnd; i++) {
        curLineLength = hViewer->textInfo->shift[currentLine + i + 1] - hViewer->textInfo->shift[currentLine + i];
        textForOut = hViewer->textInfo->text + hViewer->textInfo->shift[currentLine + i] + hViewer->scrollParam.currentPoint.horizontal;
        countNum = min(hViewer->wndParam.size.horizontal / hViewer->fontParam.charWidth, curLineLength - hViewer->scrollParam.currentPoint.horizontal);
        TextOut(hdc, 0,  i * hViewer->fontParam.charHeight, textForOut, max(0, countNum - 1));
    }
}

/** \brief
 *  ����� ������ � ���� � ����������� �� ������ ���������
 * \param HDC hdc - ��������� ��������� ���������
 * \param hViewer_t *hViewer - �������� ��������� ��� ������ � ������������� ������
 * \param PAINTSTRUCT ps - ��������� ���������
 * \return void
 */
void MyTextOut(HDC hdc, hViewer_t *hViewer, PAINTSTRUCT ps) {
    switch (hViewer->mode) {
        case USUAL:
            UsualTextOut(hdc, hViewer, ps);
            break;
        case WRAP:
            WrapTextOut(hdc, hViewer, ps);
            break;
        default:
            break;
    }
}







