#ifndef COMPONENTS_H
#define COMPONENTS_H
typedef enum error_state{
    MEMORY_ERROR,
    FILE_ERROR,
    SUCCESS,
}error_state_t;

/**
    ������ ��������� ���������� �����
    USUAL - �������
    WRAP - � ��������
*/
typedef enum mode{
    USUAL,
    WRAP,
}mode_t;


/**
    ����������
    horizontal - ��������������
    vertical - ������������
*/
typedef struct side {
    int horizontal;
    int vertical;
}side_t;

/**
    ����������
    horizontal - ��������������
    vertical - ������������
*/
typedef struct fside {
    float horizontal;
    float vertical;
}fside_t;

/**
    ��������� ������
    int charWidth - ������ �������;
    int charHeight - ������ �������;
    int cxCaps - ������� ������ �������� �������� ��������;
    int cyCaps ������� ������ �������� �������� ��������;
 */
typedef struct fontParam{
    int charWidth;
    int charHeight;
    int cxCaps;
    int cyCaps;
}fontParam_t;

/**
    ������� ����
    size - ������
 */
typedef struct wndParam{
    side_t size;
}wndParam_t;

/**
    ��������� ������ ���������
    position - �������
    currentPoint - ������� �����
    coefficient - ����������� ������
 */
typedef struct scrollParam{
    side_t position;
    side_t currentPoint;
    fside_t coefficient;
} scrollParam_t;

/**
    ��������� �������
    numOfLines - ���������� ����� � ������
    *shift - ������ �������� ����� �����
*/
typedef struct wrapInfo{
    unsigned long numOfLines;
    unsigned long *shift;
}wrapInfo_t;


/**
    ��������� ��������� ������
    *text - �����;
    numOfSymbols - ���������� �������� � ������;
    numOfLines - ���������� ����� � ������;
    maxLength - ������������ ����� ������;
    *shift - ������ �������� ����� �����;
    wrap - ��������� �������
*/
typedef struct textInfo {
    char *text;
    unsigned long numOfSymbols;
    unsigned long numOfLines;
    unsigned long maxLength;
    unsigned long *shift;
    wrapInfo_t *wrap;
}textInfo_t;


/**
    ��������� ������������� ������, ������������ � ���� ���������, ������� ���� ���������� ����
 */
typedef struct hViewer{
    mode_t mode;
    textInfo_t *textInfo;
    wndParam_t wndParam;
    scrollParam_t scrollParam;
    fontParam_t fontParam;
}hViewer_t;

/**
    ��������������� ��������� ��� ������ �� �������
    currentLine - ������� ������
    numOfLinesInWindow - ���������� �����, ������������ � ����
    numOfLinesInText - ���������� ����� � ������
    currentSymbol - ������� ������
    maxSymbolsInLine - ������������ ����� �������� � ������
 */
typedef struct infoForScroll{
    unsigned long currentLine;
    int numOfLinesInWindow;
    unsigned long numOfLinesInText;
    int currentSymbol;
    int maxSymbolsInLine;
}infoForScroll_t;

#endif //COMPONENTS_H

