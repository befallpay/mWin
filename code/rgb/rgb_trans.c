#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_b1.h"
#include "rgb_b8x.h"
#include "rgb_trans.h"


static CRgbTrans s_tRgbTrans = {{0xffff, 0x0000}, {0xffffff, 0x000000}};


int RgbBmp_SetPara(int vItem, int vPara)
{
    int vRet = -1;

    switch (vItem)
    {
    case RGB565_MAP_0:
        vRet = s_tRgbTrans.tMap565.vColor0;
        s_tRgbTrans.tMap565.vColor0 = vPara;
        break;
    case RGB565_MAP_1:
        vRet = s_tRgbTrans.tMap565.vColor1;
        s_tRgbTrans.tMap565.vColor1 = vPara;
        break;
    case RGB888_MAP_0:
        vRet = s_tRgbTrans.tMap888.vColor0;
        s_tRgbTrans.tMap888.vColor0 = vPara;
        break;
    case RGB888_MAP_1:
        vRet = s_tRgbTrans.tMap888.vColor1;
        s_tRgbTrans.tMap888.vColor1 = vPara;
        break;
    default:
        break;
    }

    return vRet;
}

int RgbBmp_GetPara(int vItem)
{
    int vRet = -1;

    switch (vItem)
    {
    case RGB565_MAP_0:
        vRet = s_tRgbTrans.tMap565.vColor0;
        break;
    case RGB565_MAP_1:
        vRet = s_tRgbTrans.tMap565.vColor1;
        break;
    case RGB888_MAP_0:
        vRet = s_tRgbTrans.tMap888.vColor0;
        break;
    case RGB888_MAP_1:
        vRet = s_tRgbTrans.tMap888.vColor1;
        break;
    default:
        break;
    }

    return vRet;
}

unsigned int rgb16to24(unsigned int vRgb16)
{
    unsigned int vColor = 0;
    CRgb565 *ptRgb565 = (CRgb565*)(&vRgb16);
    CRgb *ptRgb888 = (CRgb*)(&vColor);

    // 888
    ptRgb888->vRed = Get888Red565(ptRgb565->vRed);
    ptRgb888->vGreen = Get888Green565(ptRgb565->vGreen);
    ptRgb888->vBlue = Get888Blue565(ptRgb565->vBlue);

    return vColor;
}

unsigned int rgb24to16(unsigned int vRgb24)
{
    unsigned int vColor = 0;
    CRgb565 *ptRgb565 = (CRgb565*)(&vColor);
    CRgb *ptRgb888 = (CRgb*)(&vRgb24);

    // 565
    ptRgb565->vRed = Get565Red888(ptRgb888->vRed);
    ptRgb565->vGreen = Get565Green888(ptRgb888->vGreen);
    ptRgb565->vBlue = Get565Blue888(ptRgb888->vBlue);

    return vColor;
}

int RgbBmp_ClrTrans(int vSrcClr, int vSrcBits, int vDesBits)
{
	int vDesClr = vSrcClr;
	
	if(vSrcBits == 1){
		if(vDesBits == 16)
			vDesClr = rgb24to16(COLOR_BLACK);
		else if(vDesBits == 24)
			vDesClr = COLOR_BLACK;
	}
	else if(vSrcBits == 16){
		if(vDesBits == 1)
			vDesClr = vSrcClr == rgb24to16(COLOR_WHITE) ? 0 : 1;
		else if(vDesBits == 24)
			vDesClr = rgb16to24(vSrcClr);
	}
	else if(vSrcBits == 24){
		if(vDesBits == 1)
			vDesClr = vSrcClr == COLOR_WHITE ? 0 : 1;
		else if(vDesBits == 16)
			vDesClr = rgb24to16(vSrcClr);
	}
	
	return vDesClr;
}

int RgbBmp_DotTrans(int vRgbSrc, int vBitsSrc, int vBitsDes)
{
    int vRgbDes = 0;

    if(vBitsSrc == vBitsDes){
        vRgbDes = vRgbSrc;
        goto LB_END;
    }

    if(vBitsSrc == 1){
        // 1 to 16
        if(vBitsDes == 16){
            vRgbDes = vRgbSrc == 0 ? s_tRgbTrans.tMap565.vColor0 : s_tRgbTrans.tMap565.vColor1;
        }
        // 1 to 24
        else{
            vRgbDes = vRgbSrc == 0 ? s_tRgbTrans.tMap888.vColor0 : s_tRgbTrans.tMap888.vColor1;
        }
    }
    else if(vBitsSrc == 16){
        // 16 to 1, to do...
        if(vBitsDes == 1){
            vRgbDes = vRgbSrc == s_tRgbTrans.tMap565.vColor1 ? 1 : 0;
        }
        // 16 to 24
        else{
            vRgbDes = rgb16to24(vRgbSrc);
        }
    }
    else{
        // 24 to 1, to do...
        if(vBitsDes == 1){
            vRgbDes = vRgbSrc == s_tRgbTrans.tMap888.vColor1 ? 1 : 0;
        }
        // 24 to 16
        else{
            vRgbDes = rgb24to16(vRgbSrc);
        }
    }

LB_END:
    return vRgbDes;
}

int RgbBmp_RectTransEx(CRgbBmp *ptSrcBmp, CRgbBmp *ptDesBmp, unsigned int vOption)
{
    int vRet = 0;
    CDot tDot;
    int vColor;

    if(vOption & RBOPT_JUDGE){
        if((RgbBmp_Valid(ptSrcBmp) != 0) || (RgbBmp_Valid(ptDesBmp) != 0)){
            vRet = -1;
            goto LB_END;
        }
        if((ptSrcBmp->tRect.vWidth != ptDesBmp->tRect.vWidth) || (ptSrcBmp->tRect.vHeight != ptDesBmp->tRect.vHeight)){
            vRet = -2;
            goto LB_END;
        }
    }

    if(ptSrcBmp->vClrBits == ptDesBmp->vClrBits){
        memcpy(ptDesBmp->szFrameBuf, ptSrcBmp->szFrameBuf, ptDesBmp->tRect.vHeight*ptDesBmp->vRealWidth);
    }
    else{
        for(tDot.vY=0; tDot.vY<ptSrcBmp->tRect.vHeight; tDot.vY++){
            for(tDot.vX=0; tDot.vX<ptSrcBmp->tRect.vWidth; tDot.vX++){
                vColor = RgbBmp_GetDotEx(ptSrcBmp, &tDot, 0);
                vColor = RgbBmp_DotTrans(vColor, ptSrcBmp->vClrBits, ptDesBmp->vClrBits);
                RgbBmp_SetDotEx(ptDesBmp, &tDot, vColor, 0);
            }
        }
    }

LB_END:
    return vRet;
}

int RgbBmp_TransEx(CRgbBmp *ptMainBmp, CRgbBmp *ptSubBmp, unsigned int vOption)
{
    int vRet = 0;
    CDot tSubDot, tMainDot;
    int vColor;
    CRgbBmp tRltBmp = {0};

    if(vOption & RBOPT_JUDGE){
        if((RgbBmp_Valid(ptSubBmp) != 0) || (RgbBmp_Valid(ptMainBmp) != 0)){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_RectValid(ptSubBmp->tRect, ptMainBmp->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
    }
    tRltBmp = RgbBmp_Copy(ptSubBmp);
    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltRect(&ptMainBmp->tRect, &tRltBmp.tRect);
    }

    tMainDot.vY = tRltBmp.tRect.vTop;
    for(tSubDot.vY = 0; tSubDot.vY<tRltBmp.tRect.vHeight; tSubDot.vY++, tMainDot.vY++)
    {
        tMainDot.vX = tRltBmp.tRect.vLeft;
        for(tSubDot.vX=0; tSubDot.vX<tRltBmp.tRect.vWidth; tSubDot.vX++, tMainDot.vX++)
        {
            vColor = RgbBmp_GetDotEx(ptMainBmp, &tMainDot, 0);
            vColor = RgbBmp_DotTrans(vColor, ptMainBmp->vClrBits, tRltBmp.vClrBits);
            RgbBmp_SetDotEx(&tRltBmp, &tSubDot, vColor, 0);
        }
    }

LB_END:
    return vRet;
}

int RgbBmp_Invert(CRgbBmp *ptBmp)
{
    int vRet = 0;
    int vHeight = ptBmp->tRect.vHeight;
    int vWidth = ptBmp->vRealWidth;
    unsigned char *szFbBuf = ptBmp->szFrameBuf;
    unsigned char *szLineBuf = NULL;
    int i;

    szLineBuf = malloc(ptBmp->vRealWidth);
    if(szLineBuf == NULL){
		vRet = -1;
		goto LB_END;
	}

    for(i=0; i<(vHeight>>1); i++){
        memcpy(szLineBuf, szFbBuf+i*vWidth, vWidth);
        memcpy(szFbBuf+i*vWidth, szFbBuf+(vHeight-i-1)*vWidth, vWidth);
        memcpy(szFbBuf+(vHeight-i-1)*vWidth, szLineBuf, vWidth);
    }
    free(szLineBuf);

LB_END:
	return vRet;
}

int RgbBmp_Reverse(CRgbBmp *ptBmp)
{
    int vHeight;
    int vWidth;
    int vBits;
    unsigned char *szFbBuf = NULL;
    int i, j, index;
    CDot tDot;

    szFbBuf = ptBmp->szFrameBuf;
    vHeight = ptBmp->tRect.vHeight;

    if(ptBmp->vClrBits == 1){
        vWidth = ptBmp->tRect.vWidth/8;
        vBits = ptBmp->tRect.vWidth%8;
    }
    else{
        vWidth = ptBmp->tRect.vWidth*(ptBmp->vClrBits>>3);
        vBits = 0;
    }
    if(vWidth != 0){
        for(i=0; i<vHeight; i++){
            for(j=0; j<vWidth; j++){
                index = i*ptBmp->vRealWidth+j;
                szFbBuf[index] = ~szFbBuf[index];
            }
        }
    }
    if((ptBmp->vClrBits == 1) && (vBits != 0)){
        for(i=0; i<vHeight; i++){
            for(j=0; j<vBits; j++){
                tDot.vX=vWidth*8+j;
                tDot.vY=i;
                Bmp1_ReverseDot(ptBmp, &tDot);
            }
        }
    }

    return 0;
}

int RgbBmp_RotateEx(CRgbBmp *ptBmp, int vDegree, unsigned int vOption)
{
    int vRet = 0;

    if(vOption & RBOPT_JUDGE){
        if(RgbBmp_Valid(ptBmp) != 0){
            vRet = -1;
            goto LB_END;
        }
        if((vDegree != 90) && (vDegree != 180) && (vDegree != 270)){
            vRet = -2;
            goto LB_END;
        }
    }
    if(ptBmp->vClrBits == 1){
        Bmp1_Rotate(ptBmp, vDegree);
    }
    else{
        Bmp8X_Rotate(ptBmp, vDegree);
    }

LB_END:
    return vRet;
}

int RgbBmp_AlignEx(CRgbBmp *ptBmp, unsigned int vOption)
{
    int vRet = 0;
    CDot tDot = {0};
    int vClr;

    if(vOption & RBOPT_JUDGE){
        if(RgbBmp_Valid(ptBmp) != 0){
            vRet = -1;
            goto LB_END;
        }
        if((ptBmp->tRect.vWidth%4 != 0) || (ptBmp->vClrBits != 1)){
            vRet = -2;
            goto LB_END;
        }
    }
    for(tDot.vY=ptBmp->tRect.vHeight-1; tDot.vY>=0; tDot.vY--){
        for(tDot.vX=ptBmp->tRect.vWidth-1; tDot.vX>=0; tDot.vX--){
            vClr = Bmp1_GetBit(ptBmp, &tDot);
            Bmp1_SetDot(ptBmp, &tDot, vClr);
        }
    }

LB_END:
    return vRet;
}

int RgbBmp_JoinTransEx(CRgbBmp *ptBmp1, CRgbBmp *ptBmp8x, unsigned int vOption)
{
    int vRet = 0;
    CDot tSubDot, tMainDot;
    CRgbBmp tRltBmp = {0};
    int vColor;

    if(vOption & RBOPT_JUDGE){
        if((RgbBmp_Valid(ptBmp1) != 0) || (RgbBmp_Valid(ptBmp8x) != 0)){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_RectValid(ptBmp1->tRect, ptBmp8x->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
    }
    tRltBmp = RgbBmp_Copy(ptBmp1);
    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltRect(&ptBmp8x->tRect, &tRltBmp.tRect);
    }
    tMainDot.vY = tRltBmp.tRect.vTop;
    for(tSubDot.vY = 0; tSubDot.vY<tRltBmp.tRect.vHeight; tSubDot.vY++, tMainDot.vY++)
    {
        tMainDot.vX = tRltBmp.tRect.vLeft;
        for(tSubDot.vX=0; tSubDot.vX<tRltBmp.tRect.vWidth; tSubDot.vX++, tMainDot.vX++)
        {
            vColor = RgbBmp_GetDotEx(&tRltBmp, &tSubDot, 0);
            if(vColor == 1){
                vColor = RgbBmp_DotTrans(vColor, tRltBmp.vClrBits, ptBmp8x->vClrBits);
                RgbBmp_SetDotEx(ptBmp8x, &tMainDot, vColor, 0);
            }
        }
    }

LB_END:
    return vRet;
}

CRgbBmp RgbBmp_Multiple(CRgbBmp *ptBmp, int vWidthGain, int vHeightGain)
{
    CRgbBmp tMulBmp = {0};
    CRtg tRect;
    CDot tSrcDot;
    int vColor;

    tRect.vLeft = ptBmp->tRect.vLeft;
    tRect.vTop = ptBmp->tRect.vTop;
    tRect.vWidth = ptBmp->tRect.vWidth * vWidthGain;
    tRect.vHeight = ptBmp->tRect.vHeight * vHeightGain;
    tMulBmp = RgbBmp_New(ptBmp->vClrBits, 1, 0, &tRect, NULL);
    if(RgbBmp_Valid(&tMulBmp) != 0){
        goto LB_END;
    }
    tRect.vWidth = vWidthGain;
    tRect.vHeight = vHeightGain;
    for(tSrcDot.vY=0; tSrcDot.vY<ptBmp->tRect.vHeight; tSrcDot.vY++){
        for(tSrcDot.vX=0; tSrcDot.vX<ptBmp->tRect.vWidth; tSrcDot.vX++){
            vColor = RgbBmp_GetDot(ptBmp, &tSrcDot);
            tRect.vLeft = tSrcDot.vX*vWidthGain;
            tRect.vTop = tSrcDot.vY*vHeightGain;
            RgbBmp_ClearEx(&tMulBmp, &tRect, vColor, 0);
        }
    }

LB_END:
    return tMulBmp;
}

