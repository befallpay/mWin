#ifndef __RGB_H
#define __RGB_H


#ifdef __cplusplus
extern "C"
{
#endif

//--------------------rgb_base.h------------------//

typedef struct 
{
    int vX;
    int vY;
}CDot;

typedef struct 
{
    int vLeft;
    int vTop;
    int vWidth;
    int vHeight;
}CRtg;


typedef struct 
{
    int             vClrBits;
    int             vAlign;
    int             vFbFlg;
    CRtg            tRect;
    int             vRealWidth;
	unsigned char   *szFrameBuf;
}CRgbBmp;


#define RBOPT_REAL          0x0001
#define RBOPT_JUDGE         0x0002



int     RgbBmp_GetRealWidth(int vBits, int vAlign, int vWidth);
void    RgbBmp_WidthHeightSwap(CRgbBmp *ptBmp);
int     RgbBmp_Valid(CRgbBmp *ptBmp);
CRgbBmp RgbBmp_Copy(CRgbBmp *ptBmp);

CRgbBmp RgbBmp_New(int vBits, int vAlign, int vFbFlag, CRtg *ptRect, void *szFrameBuf);
int     RgbBmp_Delete(CRgbBmp *ptRgbBmp);

int RgbBmp_SetDotEx(CRgbBmp *ptBmp, CDot *ptDot, int vColor, unsigned int vOption);
int RgbBmp_GetDotEx(CRgbBmp *ptBmp, CDot *ptDot, unsigned int vOption);
int RgbBmp_WriteEx(CRgbBmp *ptSubBmp, CRgbBmp *ptMainBmp, unsigned int vOption);
int RgbBmp_ReadEx(CRgbBmp *ptMainBmp, CRgbBmp *ptSubBmp, unsigned int vOption);
int RgbBmp_ClearEx(CRgbBmp *ptBmp, CRtg *ptRect, int vColor, unsigned int vOption);

int RgbBmp_CutEx(CRgbBmp *ptBmp, CRtg *ptRect, unsigned int vOption);

#define RgbBmp_SetDot(ptBmp, ptDot, vColor)		RgbBmp_SetDotEx(ptBmp, ptDot, vColor, RBOPT_REAL|RBOPT_JUDGE)
#define RgbBmp_GetDot(ptBmp, ptDot)             RgbBmp_GetDotEx(ptBmp, ptDot, RBOPT_REAL|RBOPT_JUDGE)
#define RgbBmp_Write(ptSubBmp, ptMainBmp)       RgbBmp_WriteEx(ptSubBmp, ptMainBmp, RBOPT_REAL|RBOPT_JUDGE)
#define RgbBmp_Read(ptMainBmp, ptSubBmp)        RgbBmp_ReadEx(ptMainBmp, ptSubBmp, RBOPT_REAL|RBOPT_JUDGE)
#define RgbBmp_Clear(ptBmp, ptRect, vColor)		RgbBmp_ClearEx(ptBmp, ptRect, vColor, RBOPT_REAL|RBOPT_JUDGE)
#define RgbBmp_Cut(ptBmp, ptRect)				RgbBmp_CutEx(ptBmp, ptRect, RBOPT_REAL|RBOPT_JUDGE)

//--------------------rgb_trans.h------------------//

#define COLOR_BITS      (24)

#define COLOR_BLUE      (0x0000FF)
#define COLOR_YELLOW    (0xFFFF00)
#define COLOR_GREEN     (0x00FF00)
#define COLOR_RED       (0xFF0000)
#define COLOR_WHITE     (0xFFFFFF)
#define COLOR_BLACK     (0x000000)



enum
{
    RGB565_MAP_0 = 0,
    RGB565_MAP_1,
    RGB888_MAP_0,
    RGB888_MAP_1
};



int RgbBmp_SetPara(int vItem, int vPara);
int RgbBmp_GetPara(int vItem);

int RgbBmp_ClrTrans(int vSrcClr, int vSrcBits, int vDesBits);
int RgbBmp_DotTrans(int vRgbSrc, int vBitsSrc, int vBitsDes);

int RgbBmp_RectTransEx(CRgbBmp *ptSrcBmp, CRgbBmp *ptDesBmp, unsigned int vOption);
int RgbBmp_TransEx(CRgbBmp *ptMainBmp, CRgbBmp *ptSubBmp, unsigned int vOption);
int RgbBmp_Invert(CRgbBmp *ptBmp);
int RgbBmp_Reverse(CRgbBmp *ptBmp);
int RgbBmp_RotateEx(CRgbBmp *ptBmp, int vDegree, unsigned int vOption);
int RgbBmp_AlignEx(CRgbBmp *ptBmp, unsigned int vOption);
int RgbBmp_JoinTransEx(CRgbBmp *ptBmp1, CRgbBmp *ptBmp8x, unsigned int vOption);

CRgbBmp RgbBmp_Multiple(CRgbBmp *ptBmp, int vWidthGain, int vHeightGain);

#define RgbBmp_RectTrans(ptSrcBmp, ptDesBmp)    RgbBmp_RectTransEx(ptSrcBmp, ptDesBmp, RBOPT_JUDGE)
#define RgbBmp_Trans(ptMainBmp, ptSubBmp)       RgbBmp_TransEx(ptMainBmp, ptSubBmp, RBOPT_JUDGE|RBOPT_REAL)
#define RgbBmp_Rotate(ptBmp, vDegree)           RgbBmp_RotateEx(ptBmp, vDegree, RBOPT_REAL|RBOPT_JUDGE)

//--------------------rgb_bmp.h------------------//

#define BM_TYPE_STR         "BM"    //(0x4d42)


#pragma pack(1)

typedef struct
{
    unsigned char   szType[2];
    unsigned int    vFileSize;
    unsigned short  vReserve1;
    unsigned short  vReserve2;
    unsigned int    vDataOffset;
    unsigned int    vInfoSize;
}CBmpHead;

typedef struct
{
    int             vWidth;
    int             vHeight;
    unsigned short  vPlanes;
    unsigned short  vBitsPerPix;
    unsigned int    vCompression;
    unsigned int    vImageSize;
    unsigned int    vXPelsPerMeter;
    unsigned int    vYPelsPerMeter;
    unsigned int    vClrUsed;
    unsigned int    vClrImportant;
}CBmpMsg40;

typedef struct
{
    short           vWidth;
    short           vHeight;
    unsigned short  vPlanes;
    unsigned short  vBitsPerPix;
}CBmpMsg12;

typedef struct
{
    unsigned int vRedMask;
    unsigned int vGreenMask;
    unsigned int vBlueMask;
    unsigned int vAlphaMask;
}CBmpPaleMask;

typedef struct
{
    unsigned char vRed;
    unsigned char vGreen;
    unsigned char vBlue;
    unsigned char vAlpha;
}CBmpPaleItem;

typedef union
{
    CBmpPaleMask tMask;
    CBmpPaleItem atItem[2];
}CBmpPalette;

typedef struct
{
    CBmpMsg40   tMsg;
    CBmpPalette tPalette;
}CBmpInfo40;

typedef struct
{
    CBmpMsg12   tMsg;
    CBmpPalette tPalette;
}CBmpInfo12;

typedef struct
{
    CBmpHead    tHead;
    union
    {
        CBmpInfo40 tInfo40;
        CBmpInfo12 tInfo12;
    };
}CBmp;


#pragma pack()


typedef struct
{
    unsigned char *ptr;
    int len;
}CData;
#define CDATA_DECLARE(var)      CData var={NULL, 0}
#define CDATA_NEW(var, size)    do{var.ptr=(unsigned char*)malloc(size);var.len=(size);}while(0)
#define CDATA_DEL(var)          do{if(var.ptr!=NULL) {free(var.ptr);var.ptr=NULL;}}while(0)
#define CDATA_INVALID(var)      (var.ptr == NULL)

typedef struct
{
    unsigned int	vBits;
    unsigned int	vWidth;
	unsigned int	vHeight;
	unsigned int	vPaletteOst;
    unsigned int	vDataOst;
}CBmpBrief;



CBmpBrief RgbBmp_GetBrief(unsigned char *szBmpData);
CRgbBmp RgbBmp_Setup(unsigned char *szBmpData);
CData RgbBmp_FileData(CRgbBmp *ptRgbBmp);

//--------------------rgb_debug.h------------------//

int RgbBmp2File(const char *strBmpName, CRgbBmp *ptRgbBmp);

#define RgbBmpFile(ptRgbBmp)    RgbBmp2File("rgb.bmp", ptRgbBmp)

//--------------------rgb.h------------------//

#ifdef __cplusplus
};
#endif


#endif
