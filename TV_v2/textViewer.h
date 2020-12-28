#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H
#include "components.h"
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "scroll.h"
//#include "tviewer.h"
#include "readtext.h"

typedef char* (*function)(PCHAR, int*);
void MyReadFile(PCHAR fileName, hViewer_t *hViewer,error_state_t *error);

void CalculateShift(textInfo_t *textInfo, error_state_t *error);

void FillWrapInfo(textInfo_t *textInfo, int maxSymbolsInLine, error_state_t *error);
void FillFontInfo(hViewer_t *hViewer, TEXTMETRIC tm);
void FillTextInfo(textInfo_t *textInfo,error_state_t *error);

void UpdateWrapParams(hViewer_t *hViewer,error_state_t *error);

#endif //TEXTVIEWER_H
