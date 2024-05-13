#ifndef _RGB_BASE_H_
#define _RGB_BASE_H_


#define BMP1_PIX_SIZE       1/8
#define BMP16_PIX_SIZE      2
#define BMP24_PIX_SIZE      3

#define Bmp24_GetRealWidth4(ValidWidth)     ((((ValidWidth)*BMP24_PIX_SIZE)+3)/4*4)
#define Bmp16_GetRealWidth4(ValidWidth)     ((((ValidWidth)*BMP16_PIX_SIZE)+3)/4*4)
#define Bmp1_GetRealWidth4(ValidWidth)      (((ValidWidth)+31)/32*4)

#define Bmp24_GetRealWidth1(width)          ((width)*BMP24_PIX_SIZE)
#define Bmp16_GetRealWidth1(width)          ((width)*BMP16_PIX_SIZE)
#define Bmp1_GetRealWidth1(width)           (((width)+7)/8)


typedef struct 
{
    int vWidth;
    int vHeight;
}CPlane;

typedef struct 
{
    int vLeft;
    int vTop;
    int vRight;
    int vBottom;
}CArea;

//------------------------------EXTERN_PARAMETER_START--------------------------------//

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

//------------------------------EXTERN_PARAMETER_END--------------------------------//
#define RGBBMP_FBFLG_MAX	4


CPlane  Area2Plane(CArea tArea);
CArea   Rect2Area(CRtg tRect);

int     RgbBmp_DotValid(CDot tDot, CRtg tRect, unsigned int vOption);
int     RgbBmp_RectValid(CRtg tRectSub, CRtg tRectMain, unsigned int vOption);
void    RgbBmp_GetRltDot(CRtg *ptRect, CDot *ptDot);
void    RgbBmp_GetRltRect(CRtg *ptMain, CRtg *ptSub);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

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

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

