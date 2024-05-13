#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_b1.h"


int Bmp1_GetBit(CRgbBmp *ptBmp, CDot *ptDot)
{
    int ByteInd, BitInd, vOffset;
    unsigned char mask;
    unsigned char value;

    vOffset = ptDot->vY*ptBmp->tRect.vWidth + ptDot->vX;
    BitInd = vOffset%8;
    ByteInd = vOffset/8;
    mask = 0x80 >> BitInd;

    value = ptBmp->szFrameBuf[ByteInd] & mask;

    return value == 0 ? 0 : 1;
}

int Bmp1_SetDot(CRgbBmp *ptBmp, CDot *ptDot, int vColor)
{
    int ByteInd, BitInd;
    unsigned char mask;

    BitInd = ptDot->vX%8;
    ByteInd = ptDot->vY*ptBmp->vRealWidth + ptDot->vX/8;
    mask = 0x80 >> BitInd;

    if(vColor == 0)
        ptBmp->szFrameBuf[ByteInd] &= ~mask;
    else
        ptBmp->szFrameBuf[ByteInd] |= mask;

    return 0;
}

int Bmp1_GetDot(CRgbBmp *ptBmp, CDot *ptDot)
{
    int ByteInd, BitInd;
    unsigned char mask;
    unsigned char value;

    BitInd = ptDot->vX%8;
    ByteInd = ptDot->vY*ptBmp->vRealWidth + ptDot->vX/8;
    mask = 0x80 >> BitInd;

    value = ptBmp->szFrameBuf[ByteInd] & mask;

    return value == 0 ? 0 : 1;
}

int Bmp1_ReverseDot(CRgbBmp *ptBmp, CDot *ptDot)
{
    int ByteInd, BitInd;
    unsigned char mask;
    unsigned char vBitVal, vByteVal;

    BitInd = ptDot->vX%8;
    ByteInd = ptDot->vY*ptBmp->vRealWidth + ptDot->vX/8;
    mask = 0x80 >> BitInd;

    vByteVal = ptBmp->szFrameBuf[ByteInd];
    vBitVal = vByteVal & mask;
    if(vBitVal == 0)
        vByteVal |= mask;
    else
        vByteVal &= (~mask);
    ptBmp->szFrameBuf[ByteInd] = vByteVal;

    return 0;
}

CBmp1Split Bmp1_Split(CRtg tRect, int vReadFlg)
{
    CBmp1Split tSplit = {0};

    tSplit.vBytes = tRect.vWidth;
    tSplit.vHeadBits = tRect.vLeft%8;
    if(tSplit.vHeadBits != 0){
        tSplit.vBytes -= (8-tSplit.vHeadBits);
    }
    if(tSplit.vBytes >= 0){
        tSplit.vTailBits = tSplit.vBytes % 8;
        tSplit.vBytes = tSplit.vBytes / 8;
    }
    else{
        tSplit.vTailBits = (signed char)(tSplit.vBytes);
        tSplit.vBytes = 0;
    }
    if(vReadFlg != 0){
        tSplit.vBytes = tRect.vWidth/8;
    }

    return tSplit;
}

int Bmp1_Write(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes)
{
    CBmp1Split tSplit;
    unsigned char *szBufSrc = NULL;
    unsigned char *szBufDes = NULL;
    unsigned char *szSrcInd = NULL;
    unsigned char *szDesInd = NULL;
    unsigned char vTem;
    unsigned char vLeft=0, vRight=0, vTail=0;
    int i, j;

    tSplit = Bmp1_Split(ptBmpSrc->tRect, 0);
    szBufSrc = ptBmpSrc->szFrameBuf;
    szBufDes = ptBmpDes->szFrameBuf+ptBmpDes->vRealWidth*ptBmpSrc->tRect.vTop + (ptBmpSrc->tRect.vLeft>>3);
    for(i=0; i<ptBmpSrc->tRect.vHeight; i++)
    {
        szSrcInd = szBufSrc;
        szDesInd = szBufDes;
        if(tSplit.vHeadBits == 0){
            if(tSplit.vBytes != 0){
                memcpy(szDesInd, szSrcInd, tSplit.vBytes);
                szSrcInd += tSplit.vBytes;
                szDesInd += tSplit.vBytes;
            }
            if(tSplit.vTailBits != 0){
                vLeft = (*szSrcInd) & (0xff << (8-tSplit.vTailBits));
                vRight = (*szDesInd) & (0xff >> tSplit.vTailBits);
                *szDesInd = vLeft | vRight;
            }
        }
        else if(tSplit.vTailBits < 0){
            vLeft = 8-tSplit.vHeadBits;
            vRight = 8+tSplit.vTailBits;
            vTail = *szDesInd;
            vTem = vTail & (0xff << vLeft);
            vTem |= (vTail & (0xff >> vRight));
            vLeft = 8-ptBmpSrc->tRect.vWidth;
            vTail = *szSrcInd;
            vTail &= (0xff << vLeft);
            vTail >>= tSplit.vHeadBits;
            *szDesInd = vTem | vTail;
        }
        else{
            vRight = tSplit.vHeadBits;
            vLeft = 8-tSplit.vHeadBits;
            vTem = (*szDesInd) & (0xff << vLeft);
            vTail = ((*szSrcInd) & (0xff << vRight)) >> vRight;
            *szDesInd = vTem|vTail;
            szDesInd++;

            for(j=0; j<tSplit.vBytes; j++){
                vTem = szSrcInd[0] << vLeft;
                vTem |= (szSrcInd[1] >> vRight);
                *szDesInd = vTem;

                szSrcInd ++;
                szDesInd ++;
            }
            if(tSplit.vTailBits != 0){
                vTail = szDesInd[0] & (0xff >> tSplit.vTailBits);
                if(tSplit.vTailBits > vRight){
                    vTem = szSrcInd[0] << vLeft;
                    vTem |= (szSrcInd[1] >> vRight);
                    vTem &= (0xff << (8-tSplit.vTailBits));
                }
                else if(tSplit.vTailBits == vRight){
                    vTem = szSrcInd[0] << vLeft;
                }
                else{
                    vTem = szSrcInd[0] & (0xff << (vRight - tSplit.vTailBits));
                    vTem = vTem << vLeft;
                }
                szDesInd[0] = vTem | vTail;
            }
        }
        szBufSrc += ptBmpSrc->vRealWidth;
        szBufDes += ptBmpDes->vRealWidth;
    }

    return 0;
}

int Bmp1_Read(CRgbBmp *ptBmpSrc, CRgbBmp *ptBmpDes)
{
    CBmp1Split tSplit;
    unsigned char *szBufSrc = NULL;
    unsigned char *szBufDes = NULL;
    unsigned char *szSrcInd = NULL;
    unsigned char *szDesInd = NULL;
    unsigned char vTem;
    unsigned char vLeft=0, vRight=0, vTail=0;
    int i, j;

    tSplit = Bmp1_Split(ptBmpDes->tRect, 1);
    szBufSrc = ptBmpSrc->szFrameBuf+ptBmpSrc->vRealWidth*ptBmpDes->tRect.vTop + (ptBmpDes->tRect.vLeft>>3);
    szBufDes = ptBmpDes->szFrameBuf;
    for(i=0; i<ptBmpDes->tRect.vHeight; i++)
    {
        szSrcInd = szBufSrc;
        szDesInd = szBufDes;
        if(tSplit.vHeadBits == 0){
            if(tSplit.vBytes != 0){
                memcpy(szDesInd, szSrcInd, tSplit.vBytes);
                szSrcInd += tSplit.vBytes;
                szDesInd += tSplit.vBytes;
            }
            if(tSplit.vTailBits != 0){
                vTem = (*szSrcInd) & (0xff << (8-tSplit.vTailBits));
                *szDesInd = vTem;
            }
        }
        else if(tSplit.vTailBits < 0){
            vLeft = tSplit.vHeadBits;
            vRight = tSplit.vHeadBits-tSplit.vTailBits;
            vTem = *szSrcInd;
            vTem = vTem << vLeft;
            vTem = vTem & (0xff << vRight);
            *szDesInd = vTem;
        }
        else{
            vLeft = tSplit.vHeadBits;
            vRight = 8-tSplit.vHeadBits;
            for(j=0; j<tSplit.vBytes; j++){
                vTem = szSrcInd[0] << vLeft;
                vTem |= (szSrcInd[1] >> vRight);
                *szDesInd = vTem;

                szSrcInd ++;
                szDesInd ++;
            }
            if(tSplit.vTailBits != tSplit.vHeadBits){
                vTail = szSrcInd[0] << vLeft;
                if(tSplit.vTailBits > tSplit.vHeadBits){
                    vTem = vTail & (0xff << (8-(tSplit.vTailBits-tSplit.vHeadBits)));
                }
                else if(tSplit.vTailBits == 0){
                    vTem = vTail;
                }
                else{
                    vTem = szSrcInd[1] & (0xff << (8-tSplit.vTailBits));
                    vTem >>= (8-tSplit.vHeadBits);
                    vTem |= vTail;
                }
                szDesInd[0] = vTem;
            }
        }

        szBufSrc += ptBmpSrc->vRealWidth;
        szBufDes += ptBmpDes->vRealWidth;
    }

    return 0;
}

int Bmp1_Clear(CRgbBmp *ptBmp, CRtg *ptRect, int vColor)
{
    CBmp1Split tSplit;
    unsigned char *szBuf, *szLine;
    int i, j;
    unsigned char mask;

    tSplit = Bmp1_Split(*ptRect, 0);
    szLine = ptBmp->szFrameBuf + ptRect->vTop*ptBmp->vRealWidth + (ptRect->vLeft>>3);
    for(i=0; i<ptRect->vHeight; i++){
        szBuf = szLine;
        if(tSplit.vHeadBits != 0){
            if(tSplit.vTailBits < 0){
                mask = 0x80 >> tSplit.vHeadBits;
                if(vColor == 0){
                    for(j=0; j<ptRect->vWidth; j++){
                        szBuf[0] &= (~mask);
                        mask >>= 1;
                    }
                }
                else{
                    for(j=0; j<ptRect->vWidth; j++){
                        szBuf[0] |= (mask);
                        mask >>= 1;
                    }
                }
                szLine += ptBmp->vRealWidth;
                continue;
            }
            else{
                if(vColor == 0){
                    mask = 0xff << (8-tSplit.vHeadBits);
                    szBuf[0] &= mask;
                }
                else{
                    mask = 0xff >> tSplit.vHeadBits;
                    szBuf[0] |= mask;
                }
            }
            szBuf += 1;
        }
        if(tSplit.vBytes != 0){
            if(vColor == 0){
                memset(szBuf, 0, tSplit.vBytes);
            }
            else{
                memset(szBuf, 0xff, tSplit.vBytes);
            }
            szBuf += tSplit.vBytes;
        }
        if(tSplit.vTailBits > 0){
            if(vColor == 0){
                mask = 0xff >> (tSplit.vTailBits);
                szBuf[0] &= mask;
            }
            else{
                mask = 0xff << (8-tSplit.vTailBits);
                szBuf[0] |= mask;
            }
            szBuf += 1;
        }
        szLine += ptBmp->vRealWidth;
    }

    return 0;
}

int Bmp1_Rotate(CRgbBmp *ptBmp, int vDegree)
{
    int vRet = 0;
    CRgbBmp tRotate = {0};
    CDot tSrcDot, tDesDot;
    int vColor;
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
        for(tSrcDot.vY=0; tSrcDot.vY<ptBmp->tRect.vHeight; tSrcDot.vY++){
            for(tSrcDot.vX=0; tSrcDot.vX<ptBmp->tRect.vWidth; tSrcDot.vX++){
                vColor = Bmp1_GetDot(ptBmp, &tSrcDot);
                if(vColor != 0){
                    tDesDot.vX = ptBmp->tRect.vWidth-1-tSrcDot.vX;
                    tDesDot.vY = ptBmp->tRect.vHeight-1-tSrcDot.vY;
                    Bmp1_SetDot(&tRotate, &tDesDot, vColor);
                }
            }
        }
    }
    else if(vDegree == 90){
        for(tSrcDot.vY=0; tSrcDot.vY<ptBmp->tRect.vHeight; tSrcDot.vY++){
            for(tSrcDot.vX=0; tSrcDot.vX<ptBmp->tRect.vWidth; tSrcDot.vX++){
                vColor = Bmp1_GetDot(ptBmp, &tSrcDot);
                if(vColor != 0){
                    tDesDot.vX = tSrcDot.vY;
                    tDesDot.vY = ptBmp->tRect.vWidth-1-tSrcDot.vX;
                    Bmp1_SetDot(&tRotate, &tDesDot, vColor);
                }
            }
        }
    }
    else{
        vRet = -2;
        goto LB_END;
    }
    if((vDegree == 90) || (vDegree == 270)){
        RgbBmp_WidthHeightSwap(ptBmp);
    }
    memcpy(ptBmp->szFrameBuf, tRotate.szFrameBuf, tRotate.tRect.vHeight*tRotate.vRealWidth);

LB_END:
    RgbBmp_Delete(&tRotate);
    return vRet;
}

