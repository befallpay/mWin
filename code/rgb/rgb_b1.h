#ifndef _RGB_B1_H
#define _RGB_B1_H


typedef struct
{
    signed char     vHeadBits;
    signed char     vTailBits;
    signed short    vBytes;
}CBmp1Split;

int Bmp1_GetBit(CRgbBmp *ptBmp, CDot *ptDot);
int Bmp1_SetDot(CRgbBmp *ptBmp, CDot *ptDot, int vColor);
int Bmp1_GetDot(CRgbBmp *ptBmp, CDot *ptDot);
int Bmp1_ReverseDot(CRgbBmp *ptBmp, CDot *ptDot);

int Bmp1_Write(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes);
int Bmp1_Read(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes);
int Bmp1_Clear(CRgbBmp *ptBmp, CRtg *ptRect, int vColor);

int Bmp1_Rotate(CRgbBmp *ptBmp, int vDegree);


#endif

