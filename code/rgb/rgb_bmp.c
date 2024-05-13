#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_trans.h"
#include "rgb_bmp.h"


CBmpBrief RgbBmp_GetBrief(unsigned char *szBmpData)
{
    CBmpBrief tBrief = {0};
    CBmp *ptBmp = (CBmp*)szBmpData;
    int vRet = 0;

    if(memcmp(ptBmp->tHead.szType, BM_TYPE_STR, sizeof(ptBmp->tHead.szType)) != 0){
        vRet = -1;
        goto LB_END;
    }

	tBrief.vDataOst = ptBmp->tHead.vDataOffset;
    if(ptBmp->tHead.vInfoSize == 12){
        tBrief.vWidth = ptBmp->tInfo12.tMsg.vWidth;
        tBrief.vHeight = ptBmp->tInfo12.tMsg.vHeight;
        tBrief.vBits = ptBmp->tInfo12.tMsg.vBitsPerPix;
        tBrief.vPaletteOst = (unsigned char*)(ptBmp->tInfo12.tPalette.atItem)-szBmpData;
	}
    else
    {
        tBrief.vWidth = ptBmp->tInfo40.tMsg.vWidth;
        tBrief.vHeight = ptBmp->tInfo40.tMsg.vHeight;
        tBrief.vBits = ptBmp->tInfo40.tMsg.vBitsPerPix;
        tBrief.vPaletteOst = (unsigned char*)(ptBmp->tInfo40.tPalette.atItem)-szBmpData;
    }

LB_END:
    if(vRet != 0){
        memset(&tBrief, 0, sizeof(CBmpBrief));
    }
    return tBrief;
}

CRgbBmp RgbBmp_Setup(unsigned char *szBmpData)
{
    CRgbBmp tRgb = {0};
    CBmpBrief tBrief = {0};
	CBmpPaleItem *ptItem = NULL;
	int vInvFlg = 0;
    int vRet = 0;

    tBrief = RgbBmp_GetBrief(szBmpData);
	if(tBrief.vBits == 0){
		vRet = -1;
		goto LB_END;
	}

	if(tBrief.vBits == 1){
		ptItem = (CBmpPaleItem*)(szBmpData+tBrief.vPaletteOst);
		if((ptItem->vRed == 0) && (ptItem->vGreen == 0) && (ptItem->vBlue == 0)){
			vInvFlg = 1;
		}
	}

    tRgb.tRect.vWidth = tBrief.vWidth;
	tRgb.tRect.vHeight = tBrief.vHeight;
    tRgb = RgbBmp_New(tBrief.vBits, 4, 1, &tRgb.tRect, szBmpData+tBrief.vDataOst);
    if(RgbBmp_Valid(&tRgb) != 0){
        vRet = -2;
        goto LB_END;
    }
    RgbBmp_Invert(&tRgb);
    if(vInvFlg == 1){
		RgbBmp_Reverse(&tRgb);
	}

LB_END:
    if(vRet != 0){
        memset(&tRgb, 0, sizeof(CRgbBmp));
    }
    return tRgb;
}

CData RgbBmp_FileData(CRgbBmp *ptRgbBmp)
{
    CDATA_DECLARE(tBmpData);
    int vRealWidth, vImgSize, vInfoSize, i;
    unsigned char *szFileData = NULL;
    CBmp *ptBmp = NULL;


    vRealWidth = RgbBmp_GetRealWidth(ptRgbBmp->vClrBits, 4, ptRgbBmp->tRect.vWidth);
    vImgSize = vRealWidth*ptRgbBmp->tRect.vHeight;
    vInfoSize = sizeof(CBmpHead)+sizeof(CBmpMsg40);
    if(ptRgbBmp->vClrBits == 1){
        vInfoSize += 2*sizeof(CBmpPaleItem);
    }
    else if(ptRgbBmp->vClrBits == 16){
        vInfoSize += sizeof(CBmpPaleMask);
    }
    CDATA_NEW(tBmpData, vInfoSize + vImgSize);
    if(CDATA_INVALID(tBmpData)){
        goto LB_END;
    }
    szFileData = tBmpData.ptr;
    ptBmp = (CBmp*)(tBmpData.ptr);

    memcpy(ptBmp->tHead.szType, BM_TYPE_STR, sizeof(ptBmp->tHead.szType));
    ptBmp->tHead.vFileSize = vInfoSize + vImgSize;
    ptBmp->tHead.vReserve1 = 0;
    ptBmp->tHead.vReserve2 = 0;
    ptBmp->tHead.vDataOffset = vInfoSize;
    ptBmp->tHead.vInfoSize = 40;

    ptBmp->tInfo40.tMsg.vWidth = ptRgbBmp->tRect.vWidth;
    ptBmp->tInfo40.tMsg.vHeight = ptRgbBmp->tRect.vHeight;
    ptBmp->tInfo40.tMsg.vPlanes = 1;
    ptBmp->tInfo40.tMsg.vBitsPerPix = ptRgbBmp->vClrBits;
    if(ptRgbBmp->vClrBits == 16)
        ptBmp->tInfo40.tMsg.vCompression = 3;
    else
        ptBmp->tInfo40.tMsg.vCompression = 0;
    ptBmp->tInfo40.tMsg.vImageSize = vImgSize;
    ptBmp->tInfo40.tMsg.vXPelsPerMeter = 0;
    ptBmp->tInfo40.tMsg.vYPelsPerMeter = 0;
    ptBmp->tInfo40.tMsg.vClrUsed = 0;
    ptBmp->tInfo40.tMsg.vClrImportant = 0;

    if(ptRgbBmp->vClrBits == 1){
        ptBmp->tInfo40.tPalette.atItem[0].vRed = 0xff;
        ptBmp->tInfo40.tPalette.atItem[0].vGreen = 0xff;
        ptBmp->tInfo40.tPalette.atItem[0].vBlue = 0xff;
        ptBmp->tInfo40.tPalette.atItem[0].vAlpha = 0x00;

        ptBmp->tInfo40.tPalette.atItem[1].vRed = 0x00;
        ptBmp->tInfo40.tPalette.atItem[1].vGreen = 0x00;
        ptBmp->tInfo40.tPalette.atItem[1].vBlue = 0x00;
        ptBmp->tInfo40.tPalette.atItem[1].vAlpha = 0x00;
    }
    else{
        ptBmp->tInfo40.tPalette.tMask.vRedMask = MASK_RED_565;
        ptBmp->tInfo40.tPalette.tMask.vGreenMask = MASK_GREEN_565;
        ptBmp->tInfo40.tPalette.tMask.vBlueMask = MASK_BLUE_565;
        ptBmp->tInfo40.tPalette.tMask.vAlphaMask = 0;
    }

    for(i=0; i<ptRgbBmp->tRect.vHeight; i++){
        memcpy(szFileData+vInfoSize+i*vRealWidth,
                ptRgbBmp->szFrameBuf+(ptRgbBmp->tRect.vHeight-i-1)*ptRgbBmp->vRealWidth,
                ptRgbBmp->vRealWidth);
    }

LB_END:
    return tBmpData;
}

