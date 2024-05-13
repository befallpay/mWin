#include <string.h>
#include "app.h"
#include "rgb.h"
#include "rgb_test.h"


int test_rgb_debug(void)
{
	CRgbBmp tBmp;

	tBmp.tRect.vLeft = 0;
	tBmp.tRect.vTop = 0;
	tBmp.tRect.vWidth = 320;
	tBmp.tRect.vHeight = 240;
	tBmp = RgbBmp_New(16, 4, 0, &tBmp.tRect, NULL);
	RgbBmp_Clear(&tBmp, &tBmp.tRect, RgbBmp_ClrTrans(COLOR_YELLOW, 24, 16));
	RgbBmp2File("yellow.bmp", &tBmp);
	RgbBmp_Delete(&tBmp);

	return 0;
}


