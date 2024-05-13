#ifndef _RGB_BMP_H
#define _RGB_BMP_H


//------------------------------EXTERN_PARAMETER_START--------------------------------//

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

//------------------------------EXTERN_PARAMETER_END--------------------------------//

//------------------------------EXTERN_FUNCTION_START--------------------------------//

CBmpBrief RgbBmp_GetBrief(unsigned char *szBmpData);
CRgbBmp RgbBmp_Setup(unsigned char *szBmpData);
CData RgbBmp_FileData(CRgbBmp *ptRgbBmp);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

