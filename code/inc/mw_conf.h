#ifndef _MW_CONF_H
#define _MW_CONF_H


#define MW_DEBUG	

#ifdef MW_DEBUG

#define mw_printf(fmt, ...)		printf(fmt, ##__VA_ARGS__)

#else

#define mw_printf(fmt, ...)

#endif


#endif
