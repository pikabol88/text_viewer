#include "textViewer.h"

/*
char*  ReadTextFromFile(PCHAR fileName, int *numOfSymbolsInText){
    char *text;
    FILE *file = fopen(fileName, "rb");
    if(file){
        fseek(file, 0 , SEEK_END);
        int numOfSymbols = ftell(file);
        rewind(file);
        text = (PCHAR)calloc(sizeof(char), numOfSymbols + 1);
        if(text){
            fread(text, sizeof(char), numOfSymbols, file);
            fclose(file);
            (*numOfSymbolsInText)=numOfSymbols;
             return text;
        } else return NULL;
    }
    return NULL;
}*/

/** \brief
 *  Чтение текста из файла в структуры с информацией о тексте
 * \param PCHAR fileName - имя файла
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 */
void MyReadFile(PCHAR fileName, hViewer_t *hViewer, error_state_t *error){
    textInfo_t  *text = (textInfo_t*)calloc(sizeof(textInfo_t),1);
    if(text){
        int num;
        text->text=ReadTextFromFile(fileName, &num);
        if(text->text){
            text->numOfSymbols=num;
            text->text[text->numOfSymbols]='\0';
            hViewer->textInfo = text;
        } else{
            *error = FILE_ERROR;
        }
    } else {
        *error = MEMORY_ERROR;
    }
}


/** \brief
 *  Заполнение поля с информацией о шрифте в основной структуре просмотрщика текста
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \param TEXTMETRIC tm - информация о шрифте
 * \return void
 *
 */
void FillFontInfo(hViewer_t *hViewer, TEXTMETRIC tm){
    hViewer->fontParam.charWidth = tm.tmAveCharWidth;
    hViewer->fontParam.cxCaps =(tm.tmPitchAndFamily & 1 ? 3 : 2) * hViewer->fontParam.charWidth / 2;
    hViewer->fontParam.charHeight = tm.tmAveCharWidth = tm.tmHeight + tm.tmExternalLeading;
}

/** \brief
 *  Заполнение полей структуры с основной информацией о тексте(для режима без верстки)
 * \param textInfo_t *textInfo - структура, содержащая информацию о тексте
 * \return void
 *
 */
void FillTextInfo(textInfo_t *textInfo, error_state_t *error){
    int countLength=0;
    textInfo->maxLength = 0;
    textInfo->numOfSymbols = strlen(textInfo->text);
    textInfo->numOfLines = 1;
    for(int i = 0; i < textInfo->numOfSymbols; i++) {
        countLength++;
        if(textInfo->text[i] == '\n' || textInfo->text[i] == '\0'){
            textInfo->numOfLines++;
            if(countLength> textInfo->maxLength){
                textInfo->maxLength = countLength;
            }
            countLength=0;
        }
    }
    CalculateShift(textInfo, error);
}

/** \brief
 *  Заполнение полей структуры с основной информацией о тексте(для режима с версткой)
 * \param textInfo_t *textInfo - структура, содержащая информацию о тексте
 * \return void
 *
 */
void FillWrapInfo(textInfo_t *textInfo, int maxSymbolsInLine, error_state_t *error){
    textInfo->wrap = (wrapInfo_t*)calloc(sizeof(wrapInfo_t),1);
    if(textInfo->wrap){
        int numOfLines = ceil((float)textInfo->maxLength/(float)maxSymbolsInLine) * textInfo->numOfLines;
        textInfo->wrap->shift  = (int*)calloc(sizeof(int), numOfLines + 1);
        if(textInfo->wrap->shift){
            textInfo->wrap->shift[0]=0;
            int numOfSymbolsInCurrentLine, numOfWrapLines = 0;
            for(int i = 0; i<textInfo->numOfLines;i++){
                numOfSymbolsInCurrentLine = textInfo->shift[i+1]-textInfo->shift[i] - 2;
                int addLines = ceil((float)numOfSymbolsInCurrentLine / (float)maxSymbolsInLine);
                for(int j = 1; j < addLines; j++){
                    textInfo->wrap->shift[j+numOfWrapLines] = textInfo->shift[i] + maxSymbolsInLine*j;
                }
                numOfWrapLines += addLines;
                if(numOfSymbolsInCurrentLine == 0){
                    numOfWrapLines++;
                }
                textInfo->wrap->shift[numOfWrapLines] = textInfo->shift[i] + numOfSymbolsInCurrentLine+2;
            }
            textInfo->wrap->numOfLines = numOfWrapLines;
        }else{
            *error = MEMORY_ERROR;
        }
    } else{
        *error = MEMORY_ERROR;
    }
}

/** \brief
 *  Вычисление сдвига для каждой строки
 * \param textInfo_t *textInfo - структура, содержащая информацию о тексте
 * \return void
 *
 */
void CalculateShift(textInfo_t *textInfo, error_state_t *error){
    textInfo->shift = (long unsigned int*)calloc(sizeof(long unsigned int), textInfo->numOfLines + 1);
    if(textInfo->shift){
        int strNum = 1, i = 0;
        textInfo->shift[0] = 0;
        while (strNum <= textInfo->numOfLines) {
            if (textInfo->text[i] == '\n' || textInfo->text[i]=='\0') {
                textInfo->shift[strNum++] = i + 1;
            }
            i++;
        }
    }else{
        *error = MEMORY_ERROR;
    }
}

/** \brief
 *  Обновление информации для верстки в основной структуре просмотрщика текста
 * \param hViewer_t *hViewer - основная структура для работы с просмотрщиком текста
 * \return void
 *
 */
void UpdateWrapParams(hViewer_t *hViewer, error_state_t *error){
    if (hViewer->mode == WRAP) {
        int curAbsolutePosition = hViewer->textInfo->wrap->shift[hViewer->scrollParam.currentPoint.vertical];
        free(hViewer->textInfo->wrap->shift);
        FillWrapInfo(hViewer->textInfo, hViewer->wndParam.size.horizontal/hViewer->fontParam.charWidth, error);
        FindScrollPosition(hViewer, curAbsolutePosition);

    }
}














