

#ifndef OS_SUPPORT_CUSTOM_H
#define OS_SUPPORT_CUSTOM_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "xprintf.h"

#define OVERRIDE_SPEEX_ALLOC
#define OVERRIDE_SPEEX_ALLOC_SCRATCH
#define OVERRIDE_SPEEX_REALLOC
#define OVERRIDE_SPEEX_FREE
#define OVERRIDE_SPEEX_FREE_SCRATCH

void *speex_alloc (int size);
void *speex_alloc_scratch (int size);
void *speex_realloc (void *ptr, int size);
void speex_free (void *ptr);
void speex_free_scratch (void *ptr);
void speex_heap_info(uint32_t* curused, uint32_t* maxused);

#define OVERRIDE_SPEEX_COPY
#define OVERRIDE_SPEEX_MOVE
#define OVERRIDE_SPEEX_MEMSET
#define SPEEX_COPY(dst, src, n) (memcpy((dst), (src), (n)*sizeof(*(dst)) + 0*((dst)-(src)) ))
#define SPEEX_MOVE(dst, src, n) (memmove((dst), (src), (n)*sizeof(*(dst)) + 0*((dst)-(src)) ))
#define SPEEX_MEMSET(dst, c, n) (memset((dst), (c), (n)*sizeof(*(dst))))

#define OVERRIDE_SPEEX_FATAL
static inline void _speex_fatal(const char *str, const char *file, int line)
{
   xprintf("Fatal (internal) error in %s, line %d: %s\n", file, line, str);
   while(1);
}

#define OVERRIDE_SPEEX_WARNING
static inline void speex_warning(const char *str)
{
#ifndef DISABLE_WARNINGS
   xprintf ("warning: %s\n", str);
#endif
}


#define OVERRIDE_SPEEX_WARNING_INT
static inline void speex_warning_int(const char *str, int val)
{
#ifndef DISABLE_WARNINGS
   xprintf ("warning: %s %d\n", str, val);
#endif
}

#define OVERRIDE_SPEEX_NOTIFY
static inline void speex_notify(const char *str)
{
#ifndef DISABLE_NOTIFICATIONS
   xprintf ("notification: %s\n", str);
#endif
}

#define OVERRIDE_SPEEX_PUTC
static inline void _speex_putc(int ch, void *file)
{
   (void)file;
   xprintf("%c", ch);
}

#define speex_fatal(str) _speex_fatal(str, __FILE__, __LINE__);
#define speex_assert(cond) {if (!(cond)) {speex_fatal("assertion failed: " #cond);}}

#ifndef RELEASE
static inline void print_vec(float *vec, int len, char *name)
{
   int i;
   xprintf ("%s ", name);
   for (i=0;i<len;i++)
      xprintf (" %f", vec[i]);
   xprintf ("\n");
}
#endif

#endif

