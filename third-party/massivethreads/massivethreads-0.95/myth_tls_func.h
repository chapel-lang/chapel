#ifndef MYTH_TLS_FUNC_H_
#define MYTH_TLS_FUNC_H_

#include "myth_tls.h"
#include "myth_sched_proto.h"
#include "myth_sched_func.h"

extern myth_internal_lock_t g_myth_tls_lock;
extern myth_tls_entry *g_myth_tls_list;
extern int g_myth_tls_list_size;
extern int g_myth_tls_key_status[MYTH_TLS_KEY_SIZE];

static inline void myth_tls_init(int nworkers)
{
	assert(real_malloc);
	myth_internal_lock_init(&g_myth_tls_lock);
	g_myth_tls_list=NULL;
	g_myth_tls_list_size=0;
	memset(g_myth_tls_key_status,0,sizeof(int)*MYTH_TLS_KEY_SIZE);
}

static inline void myth_tls_fini(void)
{
	myth_internal_lock_destroy(&g_myth_tls_lock);
	if (g_myth_tls_list)real_free(g_myth_tls_list);
	g_myth_tls_list=NULL;
	g_myth_tls_list_size=0;
}

static inline int myth_key_create_body(myth_key_t *__key,void (*__destr_function) (void *))
{
	int i;
	int ret=0;
	myth_internal_lock_lock(&g_myth_tls_lock);
	if (__destr_function){ret=EAGAIN;}
	else{
		for (i=0;i<MYTH_TLS_KEY_SIZE;i++){
			if (g_myth_tls_key_status[i]==0)break;
		}
		if (i==MYTH_TLS_KEY_SIZE){
			ret=EAGAIN;
		}
		else{
			g_myth_tls_key_status[i]=1;
			*__key=i;
		}
	}
	myth_internal_lock_unlock(&g_myth_tls_lock);
	return ret;
}

static inline int myth_key_delete_body(myth_key_t __key)
{
	int ret=0;
	myth_internal_lock_lock(&g_myth_tls_lock);
	int i;
	if (g_myth_tls_list){
		for (i=0;i<g_myth_tls_list_size;i++){
			if (g_myth_tls_list[i].key==__key){
				memmove(&g_myth_tls_list[i],&g_myth_tls_list[i+1],sizeof(myth_tls_entry)*(g_myth_tls_list_size-i-1));
				g_myth_tls_list_size--;
			}
		}
		g_myth_tls_list=real_realloc(g_myth_tls_list,sizeof(myth_tls_entry)*g_myth_tls_list_size);
	}
	if (g_myth_tls_key_status[__key]==0){ret=EINVAL;}
	else{
		g_myth_tls_key_status[__key]=0;
	}
	myth_internal_lock_unlock(&g_myth_tls_lock);
	return ret;
}

static inline void *myth_getspecific_body(myth_key_t __key)
{
	void *ret=NULL;
	myth_internal_lock_lock(&g_myth_tls_lock);
	if (g_myth_tls_key_status[__key]!=0){
		int i;
		if (g_myth_tls_list){
			for (i=0;i<g_myth_tls_list_size;i++){
				if (g_myth_tls_list[i].key==__key && g_myth_tls_list[i].th==myth_self_body()){
					ret=g_myth_tls_list[i].value;
					break;
				}
			}
		}
	}
	myth_internal_lock_unlock(&g_myth_tls_lock);
	return ret;
}

static inline int myth_setspecific_body(myth_key_t __key,void *__pointer)
{
	int ret=0;
	myth_internal_lock_lock(&g_myth_tls_lock);
	if (g_myth_tls_key_status[__key]==0){ret=EINVAL;}
	else{
		int i;
		if (g_myth_tls_list){
			for (i=0;i<g_myth_tls_list_size;i++){
				if (g_myth_tls_list[i].key==__key && g_myth_tls_list[i].th==myth_self_body()){
					g_myth_tls_list[i].value=__pointer;
					break;
				}
			}
			if (i==g_myth_tls_list_size){
				g_myth_tls_list_size++;
				g_myth_tls_list=real_realloc(g_myth_tls_list,sizeof(myth_tls_entry)*g_myth_tls_list_size);
				g_myth_tls_list[i].key=__key;
				g_myth_tls_list[i].th=myth_self_body();
				g_myth_tls_list[i].value=__pointer;
			}
		}
		else{
			g_myth_tls_list=real_malloc(sizeof(myth_tls_entry));
			g_myth_tls_list[0].key=__key;
			g_myth_tls_list[0].th=myth_self_body();
			g_myth_tls_list[0].value=__pointer;
		}
	}
	myth_internal_lock_unlock(&g_myth_tls_lock);
	return ret;
}

#endif /* MYTH_TLS_FUNC_H_ */
