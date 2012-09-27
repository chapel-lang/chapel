#include <assert.h>
#include <dlfcn.h>
#include "myth_config.h"
#include "myth_misc.h"
#include "myth_original_lib.h"

#define LOAD_FN(fn) {real_##fn=dlsym(RTLD_NEXT,#fn);assert(real_##fn);}
#define LOAD_PTHREAD_FN(fn) {real_pthread_##fn=dlsym(s_pthread_handle,"pthread_"#fn);assert(real_pthread_##fn);}

//pthread function pointers
int (*real_pthread_key_create) (pthread_key_t *,void (*)(void *));
int (*real_pthread_key_delete) (pthread_key_t);
void *(*real_pthread_getspecific)(pthread_key_t);
int (*real_pthread_setspecific) (pthread_key_t,const void *);
int (*real_pthread_create) (pthread_t *,const pthread_attr_t *,void *(*)(void *),void *);
int (*real_pthread_join) (pthread_t, void **);
int (*real_pthread_kill)(pthread_t, int);
int (*real_pthread_sigmask)(int,const sigset_t *,sigset_t *);
int (*real_pthread_mutex_init) (pthread_mutex_t *,const pthread_mutexattr_t *);
int (*real_pthread_mutex_destroy) (pthread_mutex_t *);
int (*real_pthread_mutex_lock) (pthread_mutex_t *);
int (*real_pthread_mutex_trylock) (pthread_mutex_t *);
int (*real_pthread_mutex_unlock) (pthread_mutex_t *);
int (*real_pthread_barrier_init) (pthread_barrier_t *,const pthread_barrierattr_t *, unsigned int);
int (*real_pthread_barrier_destroy) (pthread_barrier_t *);
int (*real_pthread_barrier_wait) (pthread_barrier_t *);
int (*real_pthread_setaffinity_np) (pthread_t, size_t,cpu_set_t *);
pthread_t (*real_pthread_self) (void);
int (*real_pthread_spin_init) (pthread_spinlock_t *, int);
int (*real_pthread_spin_destroy) (pthread_spinlock_t *);
int (*real_pthread_spin_lock) (pthread_spinlock_t *);
int (*real_pthread_spin_trylock) (pthread_spinlock_t *);
int (*real_pthread_spin_unlock) (pthread_spinlock_t *);

int (*real_sched_yield)(void);

void *(*real_calloc)(size_t,size_t)=NULL;
void *(*real_malloc)(size_t)=NULL;
void (*real_free)(void *)=NULL;
void *(*real_realloc)(void *,size_t)=NULL;

static void* s_pthread_handle;

//In pthread_so_path.def, LIBPTHREAD_PATH is defined as the path of libpthread.so
//pthread_so_path.def is dynamically generated during compilation
//For details, see Makefile
#include "pthread_so_path.def"

//Load original pthread functions
static void myth_get_pthread_funcs(void)
{
	static int done=0;
	if (done)return;
	done=1;
	LOAD_FN(malloc);
	LOAD_FN(calloc);
	LOAD_FN(free);
	LOAD_FN(realloc);
	LOAD_FN(sched_yield);
	//At first, check pthread functions can be loaded using RTLD_NEXT
	if (dlsym(RTLD_NEXT,"pthread_create")){
		s_pthread_handle=RTLD_NEXT;
	}
	else{
		//pthread functions are not available in RTLD_NEXT. Load by myself
		s_pthread_handle=dlopen(LIBPTHREAD_PATH,RTLD_LAZY);
		assert(s_pthread_handle);
	}

	//Basic operation
	LOAD_PTHREAD_FN(create);LOAD_PTHREAD_FN(join);LOAD_PTHREAD_FN(self);
	//TLS
	LOAD_PTHREAD_FN(key_create);LOAD_PTHREAD_FN(key_delete);
	LOAD_PTHREAD_FN(getspecific);LOAD_PTHREAD_FN(setspecific);
	//Mutex
	LOAD_PTHREAD_FN(mutex_init);LOAD_PTHREAD_FN(mutex_destroy);
	LOAD_PTHREAD_FN(mutex_lock);LOAD_PTHREAD_FN(mutex_trylock);LOAD_PTHREAD_FN(mutex_unlock);
	//Mutex attributes
	//LOAD_PTHREAD_FN(mutexattr_init);LOAD_PTHREAD_FN(mutexattr_destroy);LOAD_PTHREAD_FN(mutexattr_settype);
	//Barrier
	LOAD_PTHREAD_FN(barrier_init);LOAD_PTHREAD_FN(barrier_destroy);LOAD_PTHREAD_FN(barrier_wait);
	//Spinlock
	LOAD_PTHREAD_FN(spin_init);LOAD_PTHREAD_FN(spin_destroy);
	LOAD_PTHREAD_FN(spin_lock);LOAD_PTHREAD_FN(spin_unlock);LOAD_PTHREAD_FN(spin_trylock);
	//Misc.
	LOAD_PTHREAD_FN(setaffinity_np);
	//signal
	LOAD_PTHREAD_FN(kill);
	LOAD_PTHREAD_FN(sigmask);
}

static void myth_free_pthread_funcs(void)
{
	if (s_pthread_handle!=RTLD_NEXT){
		//Unload libpthread.so
		dlclose(s_pthread_handle);
	}
}

//I/O function pointers
int (*real_socket)(int, int, int);
int (*real_connect)(int, const struct sockaddr *, socklen_t);
int (*real_accept)(int , struct sockaddr *,socklen_t *);
int (*real_bind)(int , const struct sockaddr *, socklen_t);
int (*real_listen)(int , int);
ssize_t (*real_send)(int , const void *, size_t , int );
ssize_t (*real_recv)(int , void *, size_t , int );
int (*real_close)(int );
int (*real_fcntl)(int , int , ...);
int (*real_select)(int, fd_set*, fd_set*,fd_set *, struct timeval *);
ssize_t (*real_sendto)(int, const void *, size_t, int,const struct sockaddr *, socklen_t);
ssize_t (*real_recvfrom)(int , void *, size_t , int ,struct sockaddr *, socklen_t *);

static void myth_get_io_funcs(void)
{
	static int done=0;
	if (done)return;
	done=1;
	LOAD_FN(socket);
	LOAD_FN(connect);
	LOAD_FN(accept);
	LOAD_FN(close);
	LOAD_FN(listen);
	LOAD_FN(bind);
	LOAD_FN(select);
	LOAD_FN(send);
	LOAD_FN(recv);
	LOAD_FN(sendto);
	LOAD_FN(recvfrom);
	LOAD_FN(fcntl);
}

static void myth_free_io_funcs(void)
{
	//Do nothing
}

void __attribute__((constructor)) myth_get_original_funcs(void)
{
	myth_get_pthread_funcs();
	myth_get_io_funcs();
}

void __attribute__((destructor)) myth_free_original_funcs(void)
{
	myth_free_pthread_funcs();
	myth_free_io_funcs();
}
