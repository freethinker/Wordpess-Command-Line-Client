/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  Debug MACROS
 *
 *        Version:  1.0
 *        Created:  06/09/2011 08:19:52 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pratik Sinha (freethinker), pratik@humbug.in
 *        Company:  http://www.humbug.in/
 *
 * =====================================================================================
 */

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
