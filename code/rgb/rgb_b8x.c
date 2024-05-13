#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_b8x.h"


int Bmp8X_SetDot(CRgbBmp *ptBmp, CDot *ptDot, int vColor)
{
    int index, bytes;

    bytes = ptBmp->vClrBits >> 3;
    index = ptBmp->vRealWidth*ptDot->vY+ptDot->vX*bytes;
    memcpy(ptBmp->szFrameBuf+index, &vColor, bytes);

    return 0;
}

int Bmp8X_GetDot(CRgbBmp *ptBmp, CDot *ptDot)
{
    int index, bytes;
    int vColor = 0;

    bytes = ptBmp->vClrBits >> 3;
    index = ptBmp->vRealWidth*ptDot->vY+ptDot->vX*bytes;
    memcpy(&vColor, ptBmp->szFrameBuf+index, bytes);

    return vColor;
}

int Bmp8X_ReverseDot(CRgbBmp *ptBmp, CDot *ptDot)
{
    int i;
    int index, bytes;

    bytes = ptBmp->vClrBits >> 3;
    index = ptBmp->vRealWidth*ptDot->vY+ptDot->vX*bytes;
    for(i=0; i<bytes; i++){
        ptBmp->szFrameBuf[index+i] = ~ptBmp->szFrameBuf[index+i];
    }

    return 0;
}

int Bmp8X_Write(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes)
{
    int bytes;
    CArea tArea;
    unsigned char *szBufSrc = NULL;
    unsigned char *szBufDes = NULL;

    bytes = ptBmpSrc->vClrBits >> 3;
    tArea = Rect2Area(ptBmpSrc->tRect);
    szBufSrc = ptBmpSrc->szFrameBuf;
    szBufDes = ptBmpDes->szFrameBuf+ptBmpDes->vRealWidth*ptBmpSrc->tRect.vTop + ptBmpSrc->tRect.vLeft*bytes;
    for(; tArea.vTop<tArea.vBottom; tArea.vTop++)
    {
        memcpy(szBufDes, szBufSrc, ptBmpSrc->vRealWidth);
        szBufSrc += ptBmpSrc->vRealWidth;
        szBufDes += ptBmpDes->vRealWidth;
    }

    return 0;
}

int Bmp8X_Read(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes)
{
    int bytes;
    CArea tArea;
    unsigned char *szBufSrc = NULL;
    unsigned char *szBufDes = NULL;

    bytes = ptBmpSrc->vClrBits >> 3;
    tArea = Rect2Area(ptBmpDes->tRect);
    szBufSrc = ptBmpSrc->szFrameBuf+ptBmpSrc->vRealWidth*ptBmpDes->tRect.vTop + ptBmpDes->tRect.vLeft*bytes;
    szBufDes = ptBmpDes->szFrameBuf;
    for(; tArea.vTop<tArea.vBottom; tArea.vTop++)
    {
        memcpy(szBufDes, szBufSrc, ptBmpDes->vRealWidth);
        szBufSrc += ptBmpSrc->vRealWidth;
        szBufDes += ptBmpDes->vRealWidth;
    }

    return 0;
}

int Bmp8X_Clear(CRgbBmp *ptBmp, CRtg *ptRect, int vColor)
{
    int i, j, bytes;
    unsigned char *szBuf, *szLineBuf;

    bytes = ptBmp->vClrBits >> 3;
    szLineBuf = ptBmp->szFrameBuf + ptBmp->vRealWidth*ptRect->vTop + bytes*ptRect->vLeft;
    for(i=0; i<ptRect->vHeight; i++)
    {
        szBuf = szLineBuf;
        for(j=0; j<ptRect->vWidth; j++)
        {
            memcpy(szBuf, &vColor, bytes);
            szBuf += bytes;
        }
        szLineBuf += ptBmp->vRealWidth;
    }

    return 0;
}


int Bmp8X_Rotate(CRgbBmp *ptBmp, int vDegree)
{
    int vRet = 0;
    int i, j, a, b;
    int bytes;
    CRgbBmp tRotate = {0};

    bytes = ptBmp->vClrBits >> 3;
    tRotate.tRect = ptBmp->tRect;
    if((vDegree == 90) || (vDegree == 270)){
        RgbBmp_WidthHeightSwap(&tRotate);
    }
    tRotate = RgbBmp_New(ptBmp->vClrBits, ptBmp->vAlign, 0, &tRotate.tRect, NULL);
    if(RgbBmp_Valid(&tRotate) != 0){
        vRet = -1;
        goto LB_END;
    }
    if(vDegree == 180){
        for(i=0; i<ptBmp->tRect.vHeight; i++){
            for(j=0; j<ptBmp->tRect.vWidth; j++){
                a = ptBmp->tRect.vWidth-1-j;
                b = ptBmp->tRect.vHeight-1-i;
                memcpy(tRotate.szFrameBuf+a*tRotate.vRealWidth+b*bytes,
                    ptBmp->szFrameBuf + i*ptBmp->vRealWidth+j*bytes, bytes);
            }
        }
    }
    else{
        vRet = -2;
        goto LB_END;
    }

    if((vDegree == 90) || (vDegree == 270)){
        ptBmp->tRect = tRotate.tRect;
    }
    memcpy(ptBmp->szFrameBuf, tRotate.szFrameBuf, tRotate.tRect.vHeight*tRotate.vRealWidth);

LB_END:
    RgbBmp_Delete(&tRotate);
    return vRet;
}

