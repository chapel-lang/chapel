#ifndef _chplmem_H_
#define _chplmem_H_



void* _chpl_malloc(size_t number, size_t size, char* description);
void* _chpl_calloc(size_t number, size_t size, char* description);
void  _chpl_free(void* ptr);
void* _chpl_realloc(void* ptr, size_t number, size_t size);

#endif
