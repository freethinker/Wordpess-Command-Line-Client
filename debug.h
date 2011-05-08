#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <stdarg.h>

#ifdef ODEBUG
int humbug_odebug = 1;
#else
int humbug_odebug = 0;
#endif

#ifdef EDEBUG
int humbug_edebug = 1;
#else
int humbug_edebug = 0;
#endif


#define debug_print(fmt, ...) \
            do { if (humbug_odebug) fprintf(stdout, fmt, __VA_ARGS__); } while (0)

#define error_print(fmt, ...) \
        do { if (humbug_edebug) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)



#endif /* _DEBUG_H */
