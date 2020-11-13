#ifndef COMPONENTS_H
#define COMPONENTS_H
typedef enum error_state{
    MEMORY_ERROR,
    FILE_ERROR,
    SUCCESS,
}error_state_t;

/**
    Режимы просмотра текстового файла
    USUAL - обычный
    WRAP - с версткой
*/
typedef enum mode{
    USUAL,
    WRAP,
}mode_t;


/**
    Ориентация
    horizontal - горизонтальная
    vertical - вертикальная
*/
typedef struct side {
    int horizontal;
    int vertical;
}side_t;

/**
    Ориентация
    horizontal - горизонтальная
    vertical - вертикальная
*/
typedef struct fside {
    float horizontal;
    float vertical;
}fside_t;

/**
    Параметры шрифта
    int charWidth - ширина символа;
    int charHeight - высота символа;
    int cxCaps - средняя ширина символов верхнего регистра;
    int cyCaps средняя высота символов верхнего регистра;
 */
typedef struct fontParam{
    int charWidth;
    int charHeight;
    int cxCaps;
    int cyCaps;
}fontParam_t;

/**
    Размеры окна
    size - размер
 */
typedef struct wndParam{
    side_t size;
}wndParam_t;

/**
    Параметры полосы прокрутки
    position - позиция
    currentPoint - текущая точка
    coefficient - коэффициент скрула
 */
typedef struct scrollParam{
    side_t position;
    side_t currentPoint;
    fside_t coefficient;
} scrollParam_t;

/**
    Параметры верстки
    numOfLines - количество строк в тексте
    *shift - массив индексов начал строк
*/
typedef struct wrapInfo{
    unsigned long numOfLines;
    unsigned long *shift;
}wrapInfo_t;


/**
    Параметры просмотра текста
    *text - текст;
    numOfSymbols - количество символов в тексте;
    numOfLines - количество строк в тексте;
    maxLength - максимальная длина строки;
    *shift - массив индексов начал строк;
    wrap - параметры верстки
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
    Описатель представления текста, объединяющий в себе структуры, которые были определены выше
 */
typedef struct hViewer{
    mode_t mode;
    textInfo_t *textInfo;
    wndParam_t wndParam;
    scrollParam_t scrollParam;
    fontParam_t fontParam;
}hViewer_t;

/**
    Вспомогательная структура для работы со скрулом
    currentLine - текущая строка
    numOfLinesInWindow - количество строк, помещающихся в окне
    numOfLinesInText - количество строк в тексте
    currentSymbol - текущий символ
    maxSymbolsInLine - максимальное число символов в строке
 */
typedef struct infoForScroll{
    unsigned long currentLine;
    int numOfLinesInWindow;
    unsigned long numOfLinesInText;
    int currentSymbol;
    int maxSymbolsInLine;
}infoForScroll_t;

#endif //COMPONENTS_H

