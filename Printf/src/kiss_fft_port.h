
#ifndef KISS_FFT_PORT_H
#define KISS_FFT_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(USE_STDLIB)
#include <stdlib.h>
#else
#include <stddef.h>
void* kiss_fft_port_malloc(size_t size);
void kiss_fft_port_free(void* p);
#define KISS_FFT_MALLOC(size) kiss_fft_port_malloc(size)
#define KISS_FFT_FREE(p)      kiss_fft_port_free(p);
#endif

#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//#define FIXED_POINT 16
#if !defined(FIXED_POINT)
#define kiss_fft_scalar float
#endif

#ifdef __cplusplus
} 
#endif

#endif
