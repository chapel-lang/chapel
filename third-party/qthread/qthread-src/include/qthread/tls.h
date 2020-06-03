#ifndef TLS_H
#define TLS_H

typedef void ** qthread_key_t;

int qthread_key_create(qthread_key_t *key, void (*destructor)(void*));
int qthread_key_delete(qthread_key_t key);
void *qthread_getspecific(qthread_key_t key);
int qthread_setspecific(qthread_key_t key, const void *value); 

#endif

