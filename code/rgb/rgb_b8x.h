#ifndef _RGB_B8X_H_
#define _RGB_B8X_H_


int Bmp8X_SetDot(CRgbBmp *ptBmp, CDot *ptDot, int vColor);
int Bmp8X_GetDot(CRgbBmp *ptBmp, CDot *ptDot);

int Bmp8X_Write(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes);
int Bmp8X_Read(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes);
int Bmp8X_Clear(CRgbBmp *ptBmp, CRtg *ptRect, int vColor);

int Bmp8X_Rotate(CRgbBmp *ptBmp, int vDegree);


#endif

