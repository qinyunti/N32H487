#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint32_t s_max_used = 0;
static uint32_t s_cur_used = 0;

void *speex_alloc (int size)
{
   void* p = malloc(size+4);  /* freertos 默认配置是8字节对齐,所以预留的4字节强制类型转换不会有对齐问题 */
   memset(p,0,size);
   *(uint32_t*)p = size;  /* 记录分配的大小 用于调试使用*/
	 s_cur_used += size;
	 if(s_cur_used >= s_max_used){
		s_max_used = s_cur_used;
	 }
   return (uint8_t*)p+4;
}

void *speex_alloc_scratch (int size)
{
   /* Scratch space doesn't need to be cleared */
   void* p = malloc(size+4);  /* freertos 默认配置是8字节对齐 */
   *(uint32_t*)p = size;  /* 记录分配的大小 */
	 s_cur_used += size;
	 if(s_cur_used >= s_max_used){
		s_max_used = s_cur_used;
	 }
   return (uint8_t*)p+4;
}

void *speex_realloc (void *ptr, int size)
{
   uint32_t len;
   void* p = malloc(size+4);
   *(uint32_t*)p = size;  /* 记录分配的大小 */
   len = *(uint32_t*)((uint8_t*)ptr-4);
   memcpy((uint8_t*)p+4,ptr,len);
   free((uint8_t*)ptr-4);
	
	 s_cur_used += size;
	 s_cur_used -= len;
	 if(s_cur_used >= s_max_used){
		s_max_used = s_cur_used;
	 }
	 
   return (uint8_t*)p+4;
}

void speex_free (void *ptr)
{
	 uint32_t len = *(uint32_t*)((uint8_t*)ptr-4);
   free((uint8_t*)ptr-4);
	 s_cur_used -= len;
}

void speex_free_scratch (void *ptr)
{
	 uint32_t len = *(uint32_t*)((uint8_t*)ptr-4);
   free((uint8_t*)ptr-4);
	 s_cur_used -= len;
}

void speex_heap_info(uint32_t* curused, uint32_t* maxused){
	*curused = s_cur_used;
	*maxused = s_max_used;
}

