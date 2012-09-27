#ifndef MYTH_MALLOC_WRAPPER_H_
#define MYTH_MALLOC_WRAPPER_H_

void myth_malloc_wrapper_init(int nthreads);
void myth_malloc_wrapper_init_worker(int rank);
void myth_malloc_wrapper_fini();
void myth_malloc_wrapper_fini_worker(int rank);

void *calloc(size_t nmemb,size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr,size_t size);

#endif /* MYTH_MALLOC_WRAPPER_H_ */
