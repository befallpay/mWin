#include "rgb_input.h"
#include "rgb_base.h"
#include "rgb_bmp.h"
#include "rgb_debug.h"

#ifdef MW_DEBUG
static int SaveToFile(const char *strBmpName, void *szData, int vLen)
{
    int vRet = 0;
    FILE *fp;
    int iRes;

    fp = fopen(strBmpName, "wb+");
    if(fp == NULL){
		vRet = -1;
		goto LB_END;
	}
    iRes = fwrite(szData, 1, vLen, fp);
    if(iRes != vLen){
		vRet = -2;
		goto LB_END;
	}

LB_END:
    if(fp != NULL){
        fclose(fp);
    }
    return vRet;
}

int RgbBmp2File(const char *strBmpName, CRgbBmp *ptRgbBmp)
{
    int vRet = 0;
    int iRes;

    CDATA_DECLARE(tBmpData);

    tBmpData = RgbBmp_FileData(ptRgbBmp);
    if(CDATA_INVALID(tBmpData)){
		vRet = -1;
		goto LB_END;
	}
    iRes = SaveToFile(strBmpName, tBmpData.ptr, tBmpData.len);
    if(iRes != 0){
		vRet = -2;
		goto LB_END;
	}

LB_END:
    CDATA_DEL(tBmpData);
    return vRet;
}

#else

int RgbBmp2File(const char *strBmpName, CRgbBmp *ptRgbBmp)
{
	return 0;
}

#endif

