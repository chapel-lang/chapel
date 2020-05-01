#include <qthread/qthread.h>
#include <qthread/tls.h>

struct qt_keypair_t {
	qthread_key_t key;
	void *value;
	void (*destructor)(void*);
};
typedef struct qt_keypair_t qt_keypair_t;

struct TLS {
	int initialized;
	int nkeys;
	qt_keypair_t key_pair[];
};
typedef struct TLS TLS;

int qthread_key_create(qthread_key_t *key, void (*destructor)(void*))
{
	int i;
	int size = qthread_size_tasklocal();
	TLS *t = (TLS *)qthread_get_tasklocal(size);
	qt_keypair_t *p;

	printf("t->initialized %d\n", t->initialized);
	p = &t->key_pair[t->nkeys];
	p->key = *key;
	p->value = NULL;
	p->destructor = destructor;
	t->nkeys++;
	return 0;
}

int qthread_key_delete(qthread_key_t key)
{
	int i;
	int size = qthread_size_tasklocal();
	TLS *t = (TLS *)qthread_get_tasklocal(size);
	qt_keypair_t *p;
	for(i = 0; i < t->nkeys; i++) {
		p = &t->key_pair[i];
		if(p->key == key) {
			p->key = 0;
			p->destructor(p->value);
			return 0;
		}
	}
	return 1;
}

void *qthread_getspecific(qthread_key_t key)
{
	int i;
	int size = qthread_size_tasklocal();
	TLS *t = (TLS *)qthread_get_tasklocal(size);
	qt_keypair_t *p;

	for(i = 0; i < t->nkeys; i++) {
		p = &t->key_pair[i];
		if(p->key == key) {
			return p->value;
		}
	}
	return NULL;
}

int qthread_setspecific(qthread_key_t key, const void *value)
{
	int i;
	int size = qthread_size_tasklocal();
	TLS *t = (TLS *)qthread_get_tasklocal(size);
	qt_keypair_t *p;

	for(i = 0; i < t->nkeys; i++) {
		p = &t->key_pair[i];
		if(p->key == key) {
			p->value = (void*)value;
			return 0;
		}
	}
	return 1;
}
