#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_b1.h"
#include "rgb_b8x.h"


#define RgbBmp_BitsValid(vBits)         (((vBits) == 1) || ((vBits) == 16) || ((vBits) == 24))
#define RgbBmp_AlignValid(vAlign)       (((vAlign) == 1) || ((vAlign) == 4))
#define RgbBmp_FbFlagValid(vFbFlag)     (((vFbFlag) >= 0) && ((vFbFlag) <= RGBBMP_FBFLG_MAX))


CPlane Area2Plane(CArea tArea)
{
    CPlane tPlane;

    tPlane.vWidth = tArea.vRight-tArea.vLeft;
    tPlane.vHeight = tArea.vBottom-tArea.vTop;

    return tPlane;
}

CArea Rect2Area(CRtg tRect)
{
    CArea tArea;

    tArea.vLeft = tRect.vLeft;
    tArea.vTop = tRect.vTop;
    tArea.vRight = tRect.vLeft+tRect.vWidth;
    tArea.vBottom = tRect.vTop+tRect.vHeight;

    return tArea;
}

int RgbBmp_DotValid(CDot tDot, CRtg tRect, unsigned int vOption)
{
    int vRet = -1;

    if(vOption & RBOPT_REAL){
        if((tDot.vX >= tRect.vLeft) && (tDot.vY >= tRect.vTop)
            && (tDot.vX < tRect.vLeft+tRect.vWidth) && (tDot.vY < tRect.vTop+tRect.vHeight)){
            vRet = 0;
        }
    }
    else{
        if((tDot.vX < tRect.vWidth) && (tDot.vY < tRect.vHeight)){
            vRet = 0;
        }
    }

    return vRet;
}

int RgbBmp_RectValid(CRtg tSubRect, CRtg tMainRect, unsigned int vOption)
{
    int vRet = -1;
    CArea tSubArea, tMainArea;

    tSubArea = Rect2Area(tSubRect);
    tMainArea = Rect2Area(tMainRect);

    if(vOption & RBOPT_REAL){

        if((tSubArea.vLeft >= tMainArea.vLeft) && (tSubArea.vTop >= tMainArea.vTop) &&
            (tSubArea.vRight <= tMainArea.vRight) && (tSubArea.vBottom <= tMainArea.vBottom)){
            vRet = 0;
        }
    }
    else{
        if((tSubArea.vRight <= tMainRect.vWidth) && (tSubArea.vBottom <= tMainRect.vHeight)){
            vRet = 0;
        }
    }

    return vRet;
}

void RgbBmp_GetRltDot(CRtg *ptRect, CDot *ptDot)
{
    ptDot->vX -= ptRect->vLeft;
    ptDot->vY -= ptRect->vTop;
}

void RgbBmp_GetRltRect(CRtg *ptMain, CRtg *ptSub)
{
    ptSub->vLeft -= ptMain->vLeft;
    ptSub->vTop -= ptMain->vTop;
}

void RgbBmp_WidthHeightSwap(CRgbBmp *ptBmp)
{
    int vTem;

    vTem = ptBmp->tRect.vHeight;
    ptBmp->tRect.vHeight = ptBmp->tRect.vWidth;
    ptBmp->tRect.vWidth = vTem;

    ptBmp->vRealWidth = RgbBmp_GetRealWidth(ptBmp->vClrBits, ptBmp->vAlign, ptBmp->tRect.vWidth);
}

int RgbBmp_GetRealWidth(int vBits, int vAlign, int vWidth)
{
    int vRealWidth = 0;

    if(vAlign == 1){
        if(vBits == 1)
            vRealWidth = Bmp1_GetRealWidth1(vWidth);
        else if(vBits == 16)
            vRealWidth = Bmp16_GetRealWidth1(vWidth);
        else if(vBits == 24)
            vRealWidth = Bmp24_GetRealWidth1(vWidth);
    }
    else{
        if(vBits == 1)
            vRealWidth = Bmp1_GetRealWidth4(vWidth);
        else if(vBits == 16)
            vRealWidth = Bmp16_GetRealWidth4(vWidth);
        else if(vBits == 24)
            vRealWidth = Bmp24_GetRealWidth4(vWidth);
    }

    return vRealWidth;
}


int RgbBmp_Valid(CRgbBmp *ptBmp)
{
    int vRet = 0;

    if(!RgbBmp_BitsValid(ptBmp->vClrBits)){
        vRet = -1;
        goto LB_END;
    }

    if(!RgbBmp_AlignValid(ptBmp->vAlign)){
        vRet = -2;
        goto LB_END;
    }

    if(!RgbBmp_FbFlagValid(ptBmp->vFbFlg)){
        vRet = -3;
        goto LB_END;
    }

    if((ptBmp->tRect.vWidth <= 0) || (ptBmp->tRect.vHeight <= 0)){
        vRet = -4;
        goto LB_END;
    }

    if(ptBmp->vRealWidth <= 0){
        vRet = -5;
        goto LB_END;
    }

    if((ptBmp->vFbFlg != 2) && (ptBmp->szFrameBuf == NULL)){
        vRet = -6;
        goto LB_END;
    }

    if((ptBmp->vFbFlg == 2) && (ptBmp->szFrameBuf != NULL)){
        vRet = -7;
        goto LB_END;
    }

LB_END:
    return vRet;
}

static int RgbBmp_NewValid(int vBits, int vAlign, int vFbFlag, CRtg *ptRect, void *szFrameBuf)
{
    int vRes = 0;

    if(!RgbBmp_BitsValid(vBits)){
        vRes = -1;
        goto LB_END;
    }

    if(!RgbBmp_AlignValid(vAlign)){
        vRes = -2;
        goto LB_END;
    }

    if(!RgbBmp_FbFlagValid(vFbFlag)){
        vRes = -3;
        goto LB_END;
    }

    if((ptRect->vWidth == 0) || (ptRect->vHeight == 0)){
        vRes = -4;
        goto LB_END;
    }

    if((vFbFlag == 1) && (szFrameBuf == NULL)){
        vRes = -6;
        goto LB_END;
    }

    if((vFbFlag != 1) && (szFrameBuf != NULL)){
        vRes = -7;
        goto LB_END;
    }

LB_END:
    return vRes;
}

CRgbBmp RgbBmp_New(int vBits, int vAlign, int vFbFlag, CRtg *ptRect, void *szFrameBuf)
{
    CRgbBmp tBmp = {0};
    int vSize, vRes = 0;

    vRes = RgbBmp_NewValid(vBits, vAlign, vFbFlag, ptRect, szFrameBuf);
    if(vRes != 0){
        vRes = -1;
        goto LB_END;
    }

    tBmp.vClrBits = vBits;
    tBmp.vAlign = vAlign;
    tBmp.vFbFlg = vFbFlag;
    memcpy(&tBmp.tRect, ptRect, sizeof(CRtg));

    tBmp.vRealWidth = RgbBmp_GetRealWidth(vBits, vAlign, ptRect->vWidth);

    vSize = tBmp.vRealWidth*ptRect->vHeight;
    if(tBmp.vFbFlg == 0){
        tBmp.szFrameBuf = malloc(vSize);
        if(tBmp.szFrameBuf == NULL){
            vRes = -2;
            goto LB_END;
        }

        if(vBits == 1)
            memset(tBmp.szFrameBuf, 0x00, vSize);
        else
            memset(tBmp.szFrameBuf, 0xff, vSize);
    }
    else if(tBmp.vFbFlg == 1){
        tBmp.szFrameBuf = szFrameBuf;
    }
    else{
        tBmp.szFrameBuf = NULL;
    }

LB_END:
    if(vRes != 0){
        if((tBmp.szFrameBuf != NULL) && (tBmp.vFbFlg == 0)){
            free(tBmp.szFrameBuf);
        }
        memset(&tBmp, 0, sizeof(CRgbBmp));
    }
    return tBmp;
}

int RgbBmp_Delete(CRgbBmp *ptBmp)
{
    if(ptBmp != NULL)
    {
        if((ptBmp->szFrameBuf != NULL) && (ptBmp->vFbFlg == 0))
        {
            free(ptBmp->szFrameBuf);
        }
        memset(ptBmp, 0, sizeof(CRgbBmp));
    }

    return 0;
}

CRgbBmp RgbBmp_Copy(CRgbBmp *ptBmp)
{
    CRgbBmp tBmp;

    memcpy(&tBmp, ptBmp, sizeof(CRgbBmp));
    if(ptBmp->vFbFlg == 0)
        tBmp.vFbFlg = 3;

    return tBmp;
}

int RgbBmp_SetDotEx(CRgbBmp *ptBmp, CDot *ptDot, int vColor, unsigned int vOption)
{
    int vRet = 0;
    CDot tRltDot;

    if(vOption & RBOPT_JUDGE){
        if(RgbBmp_Valid(ptBmp) != 0){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_DotValid(*ptDot, ptBmp->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
    }
    memcpy(&tRltDot, ptDot, sizeof(CDot));

    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltDot(&ptBmp->tRect, &tRltDot);
    }

    if(ptBmp->vClrBits == 1){
        Bmp1_SetDot(ptBmp, &tRltDot, vColor);
    }
    else{
        Bmp8X_SetDot(ptBmp, &tRltDot, vColor);
    }

LB_END:
    return vRet;
}

int RgbBmp_GetDotEx(CRgbBmp *ptBmp, CDot *ptDot, unsigned int vOption)
{
    int vRet = 0;
    CDot tRltDot;

    if(vOption & RBOPT_JUDGE){
        if(RgbBmp_Valid(ptBmp) != 0){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_DotValid(*ptDot, ptBmp->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
    }
    memcpy(&tRltDot, ptDot, sizeof(CDot));

    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltDot(&ptBmp->tRect, &tRltDot);
    }

    if(ptBmp->vClrBits == 1){
        vRet = Bmp1_GetDot(ptBmp, &tRltDot);
    }
    else{
        vRet = Bmp8X_GetDot(ptBmp, &tRltDot);
    }

LB_END:
    return vRet;
}

int RgbBmp_WriteEx(CRgbBmp *ptSubBmp, CRgbBmp *ptMainBmp, unsigned int vOption)
{
    int vRet = 0;
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

        if(ptMainBmp->vClrBits != ptSubBmp->vClrBits){
            vRet = -3;
            goto LB_END;
        }
    }
    tRltBmp = RgbBmp_Copy(ptSubBmp);

    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltRect(&ptMainBmp->tRect, &tRltBmp.tRect);
    }

    if(ptSubBmp->vClrBits == 1){
        Bmp1_Write(&tRltBmp, ptMainBmp);
    }
    else{
        Bmp8X_Write(&tRltBmp, ptMainBmp);
    }

LB_END:
    return vRet;
}

int RgbBmp_ReadEx(CRgbBmp *ptMainBmp, CRgbBmp *ptSubBmp, unsigned int vOption)
{
    int vRet=0;
    CRgbBmp tRltBmp = {0};

    if(vOption & RBOPT_JUDGE){
        if((RgbBmp_Valid(ptMainBmp) != 0) || (RgbBmp_Valid(ptSubBmp) != 0)){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_RectValid(ptSubBmp->tRect, ptMainBmp->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
        if(ptSubBmp->vClrBits != ptMainBmp->vClrBits){
            vRet = -3;
            goto LB_END;
        }
    }
    tRltBmp = RgbBmp_Copy(ptSubBmp);

    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltRect(&ptMainBmp->tRect, &tRltBmp.tRect);
    }

    if(ptMainBmp->vClrBits == 1){
        Bmp1_Read(ptMainBmp, &tRltBmp);
    }
    else{
        Bmp8X_Read(ptMainBmp, &tRltBmp);
    }

LB_END:
    return vRet;
}

int RgbBmp_CutEx(CRgbBmp *ptBmp, CRtg *ptRect, unsigned int vOption)
{
    int vRet = 0;
    int vRealWidth, i;
    CRgbBmp tBmp = {0};
    unsigned char *szSrc, *szDes;

    if(vOption & RBOPT_JUDGE){
        if((ptBmp->tRect.vLeft != ptRect->vLeft) || (ptBmp->tRect.vTop != ptRect->vTop)){
            vRet = -1;
            goto LB_END;
        }
        if((ptRect->vWidth > ptBmp->tRect.vWidth) || (ptRect->vHeight > ptBmp->tRect.vHeight)){
            vRet = -2;
            goto LB_END;
        }
    }

    if(ptRect->vWidth == ptBmp->tRect.vWidth){
        memcpy(&ptBmp->tRect, ptRect, sizeof(CRtg));
        goto LB_END;
    }
    tBmp = RgbBmp_New(ptBmp->vClrBits, ptBmp->vAlign, 1, ptRect, ptBmp->szFrameBuf);
	vRealWidth = tBmp.vRealWidth;

    szSrc = ptBmp->szFrameBuf;
    szDes = tBmp.szFrameBuf;
    for(i=1; i<ptRect->vHeight; i++){
        szSrc += ptBmp->vRealWidth;
        szDes += vRealWidth;
        memcpy(szDes, szSrc, vRealWidth);
    }
    memcpy(&ptBmp->tRect, ptRect, sizeof(CRtg));
    ptBmp->vRealWidth = vRealWidth;
	RgbBmp_Delete(&tBmp);

LB_END:
    return vRet;
}

int RgbBmp_ClearEx(CRgbBmp *ptBmp, CRtg *ptRect, int vColor, unsigned int vOption)
{
    int vRet = 0;
    CRtg tRect;

    if(vOption & RBOPT_JUDGE){
        if(RgbBmp_Valid(ptBmp) != 0){
            vRet = -1;
            goto LB_END;
        }
        if(RgbBmp_RectValid(*ptRect, ptBmp->tRect, vOption) != 0){
            vRet = -2;
            goto LB_END;
        }
    }
    memcpy(&tRect, ptRect, sizeof(CRtg));

    if(vOption & RBOPT_REAL){
        RgbBmp_GetRltRect(&ptBmp->tRect, &tRect);
    }

    if(ptBmp->vClrBits == 1){
        Bmp1_Clear(ptBmp, &tRect, vColor);
    }
    else{
        Bmp8X_Clear(ptBmp, &tRect, vColor);
    }

LB_END:
    return vRet;
}


