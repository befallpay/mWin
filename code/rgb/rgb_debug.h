#ifndef _RGB_DEBUG_H
#define _RGB_DEBUG_H



//------------------------------EXTERN_FUNCTION_START--------------------------------//

int RgbBmp2File(const char *strBmpName, CRgbBmp *ptRgbBmp);

#define RgbBmpFile(ptRgbBmp)    RgbBmp2File("rgb.bmp", ptRgbBmp)

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

