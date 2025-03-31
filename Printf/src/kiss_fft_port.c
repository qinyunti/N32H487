#include <stddef.h>
#include <stdlib.h>
void* kiss_fft_port_malloc(size_t size)
{
    return malloc(size);
}

void kiss_fft_port_free(void* p)
{
    free(p);
}