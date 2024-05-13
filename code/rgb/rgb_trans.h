#ifndef _RGB_TRANS_H_
#define _RGB_TRANS_H_

//------------------------------EXTERN_PARAMETER_START--------------------------------//

#define COLOR_BITS      (24)

#define COLOR_BLUE      (0x0000FF)
#define COLOR_YELLOW    (0xFFFF00)
#define COLOR_GREEN     (0x00FF00)
#define COLOR_RED       (0xFF0000)
#define COLOR_WHITE     (0xFFFFFF)
#define COLOR_BLACK     (0x000000)

//------------------------------EXTERN_PARAMETER_END--------------------------------//

#define	MIN_ALPHA				(0)
#define	MAX_ALPHA				(31)
#define	COUNT_ALPHA_STEP		(32)
#define	MIDDLE_ALPHA			(MAX_ALPHA >> 1)
#define	ALPHA_SHIFTBIT			(5)

#define	OPAQUE					(MIN_ALPHA)
#define	TRANSPARENT 			(MAX_ALPHA)

//-------------------------RGB565---------------------------//

#define	SHIFT_RED_565           (11)
#define	SHIFT_GREEN_565         (5)
#define	SHIFT_BLUE_565          (0)

#define	MASK_RED_565            (0xF800)
#define	MASK_GREEN_565          (0x07E0)
#define	MASK_BLUE_565           (0x001F)

#define GetRed565(vColor)       (((vColor)&MASK_RED_565)>>SHIFT_RED_565)
#define GetGreen565(vColor)     (((vColor)&MASK_GREEN_565)>>SHIFT_GREEN_565)
#define GetBlue565(vColor)      (((vColor)&MASK_BLUE_565)>>SHIFT_BLUE_565)

#define	GetRGB565(vR, vG, vB)   ((unsigned short)((((vR) << SHIFT_RED_565) & MASK_RED_565) \
									| (((vG) << SHIFT_GREEN_565) & MASK_GREEN_565)	 \
									| (((vB) << SHIFT_BLUE_565) & MASK_BLUE_565)))

//---------------------------RGB888----------------------------//

#define	SHIFT_RED_888           (16)
#define	SHIFT_GREEN_888         (8)
#define	SHIFT_BLUE_888          (0)

#define	MASK_RED_888            (0xFF0000)
#define	MASK_GREEN_888          (0x00FF00)
#define	MASK_BLUE_888           (0x0000FF)

#define GetRed888(vColor)       ((((unsigned short)(vColor))&MASK_RED_888)>>SHIFT_RED_888)
#define GetGreen888(vColor)     ((((unsigned short)(vColor)&MASK_GREEN_888))>>SHIFT_GREEN_888)
#define GetBlue888(vColor)      ((((unsigned short)(vColor)&MASK_BLUE_888))>>SHIFT_BLUE_888)

#define	GetRGB888(vR, vG, vB)   ((unsigned short)(((((unsigned short)(vR)) << SHIFT_RED_888) & MASK_RED_888) \
									| ((((unsigned short)(vG)) << SHIFT_GREEN_888) & MASK_GREEN_888)	 \
									| ((((unsigned short)(vB)) << SHIFT_BLUE_888) & MASK_BLUE_888)))


//---------------------------RGB888,RGB565 trans----------------------------//

// RGB888 --> RGB565
#define SHIFT_888_RED_565       (3)
#define SHIFT_888_GREEN_565     (2)
#define SHIFT_888_BLUE_565      (3)

// RGB888-->565
#define Get565Red888(vR)        ((unsigned char)((((unsigned char)(vR)) >> SHIFT_888_RED_565)))
#define Get565Green888(vG)      ((unsigned char)((((unsigned char)(vG)) >> SHIFT_888_GREEN_565)))
#define Get565Blue888(vB)       ((unsigned char)((((unsigned char)(vB)) >> SHIFT_888_BLUE_565)))

// RGB888 --> RGB565
#define GetBitMax(bits)         ((1<<(bits))-1)

// RGB565-->888
#define Get888Red565(vR)        ((((unsigned int)(vR)) * GetBitMax(8)) / GetBitMax(5))
#define Get888Green565(vG)      ((((unsigned int)(vG)) * GetBitMax(8)) / GetBitMax(6))
#define Get888Blue565(vB)       ((((unsigned int)(vB)) * GetBitMax(8)) / GetBitMax(5))

typedef struct
{
    unsigned int  vBlue    : 5;
    unsigned int  vGreen   : 6;
    unsigned int  vRed     : 5;
    unsigned int  vReserve : 16;
}CRgb565;

typedef struct
{
    unsigned char vBlue;
    unsigned char vGreen;
    unsigned char vRed;
    unsigned char vReserve;
}CRgb;

typedef struct
{
    unsigned int vColor0;
    unsigned int vColor1;
}CRgbMap;

typedef struct
{
    CRgbMap tMap565;
    CRgbMap tMap888;
}CRgbTrans;

//------------------------------EXTERN_PARAMETER_START--------------------------------//

enum
{
    RGB565_MAP_0 = 0,
    RGB565_MAP_1,
    RGB888_MAP_0,
    RGB888_MAP_1
};

//------------------------------EXTERN_PARAMETER_END--------------------------------//

unsigned int rgb16to24(unsigned int vRgb16);
unsigned int rgb24to16(unsigned int vRgb24);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

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

//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

