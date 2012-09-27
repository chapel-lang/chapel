#ifndef MYTH_WSQUEUE_FUNC_H_
#define MYTH_WSQUEUE_FUNC_H_

#include "myth_config.h"
#include "myth_desc.h"
#include "myth_wsqueue.h"
#include "myth_wsqueue_proto.h"

//Initialize thread-local data
static inline void myth_queue_init_thread_data(myth_queue_data_t th){}
static inline void myth_queue_fini_thread_data(myth_queue_data_t th){}

//critical section for signal
static inline void myth_queue_enter_operation(myth_thread_queue_t q)
{
#ifdef USE_SIGNAL_CS
	assert(q->op_flag==0);
	q->op_flag=1;
	myth_wsqueue_wbarrier();
#endif
#ifdef USE_THREAD_CS
	real_pthread_mutex_lock(&q->mtx);
#endif
}

static inline void myth_queue_exit_operation(myth_thread_queue_t q)
{
#ifdef USE_SIGNAL_CS
	assert(q->op_flag==1);
	myth_wsqueue_wbarrier();
	q->op_flag=0;
#endif
#ifdef USE_THREAD_CS
	real_pthread_mutex_unlock(&q->mtx);
#endif
}

static inline int myth_queue_is_operating(myth_thread_queue_t q)
{
	int ret=0;
#ifdef USE_SIGNAL_CS
	ret=ret && q->op_flag;
#endif
	return ret;
}

static inline void myth_queue_init(myth_thread_queue_t q){
	myth_wsqueue_lock_init(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_ANY
	myth_internal_lock_init(&q->m_lock);
#endif
#ifdef USE_SIGNAL_CS
	q->op_flag=0;
#endif
#ifdef USE_THREAD_CS
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK);
	real_pthread_mutex_init(&q->mtx,&attr);
	pthread_mutexattr_destroy(&attr);
#endif
	q->size=INITIAL_QUEUE_SIZE;
	q->ptr=myth_malloc(sizeof(myth_thread_t)*q->size);
	memset(q->ptr,0,sizeof(myth_thread_t)*q->size);
	q->base=q->size/2;
	q->top=q->base;
}

static inline void myth_queue_fini(myth_thread_queue_t q){
	myth_queue_clear(q);
	myth_wsqueue_lock_destroy(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_ANY
	myth_internal_lock_destroy(&q->m_lock);
#endif
	myth_free(q->ptr,0);
}

static inline void myth_queue_clear(myth_thread_queue_t q)
{
	myth_queue_enter_operation(q);
#if defined USE_LOCK || defined USE_LOCK_CLEAR
	myth_internal_lock_lock(&q->m_lock);
#endif
	myth_wsqueue_lock_lock(&q->lock);
	myth_assert(q->top==q->base);
	q->base=q->size/2;
	q->top=q->base;
	myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_CLEAR
	myth_internal_lock_unlock(&q->m_lock);
#endif
	myth_queue_exit_operation(q);
}

//push/pop/peek:Owner thread operations
static inline void __attribute__((always_inline)) myth_queue_push(myth_thread_queue_t q,myth_thread_t th)
{
	myth_queue_enter_operation(q);
#if defined USE_LOCK || defined USE_LOCK_PUSH
	myth_internal_lock_lock(&q->m_lock);
#endif
	//Check
	int t=q->top;
	//read barrier
	myth_wsqueue_rbarrier();
	if (t==q->size){
		//Acquire lock
		myth_wsqueue_lock_lock(&q->lock);
		//Runqueue full?
		if (q->base==0){
			myth_assert(0);
			fprintf(stderr,"Fatal error:Runqueue overflow\n");
			abort();
			//TODO:extend runqueue
			/*myth_thread_t *newptr;
			int newsize;
			//Extend runqueue
			newsize=q->size*2;
			//Newly allocate
			newptr=myth_malloc(sizeof(myth_thread_t)*newsize);
			//copy
			memcpy(newptr+,q->ptr+base,sizeof(myth_thread_t)*(q->top-q->base+1));
			//Adjust index
			q->top=;q->base=;
			//Release that old array and replace to new one
			myth_free(q->ptr);q->ptr=newptr;q->size=newsize;*/
		}
		else{
			//Shift pointers
			int offset,offset_x2;
			offset_x2=q->size-(q->base+q->top);
			offset=offset_x2/2;if (offset_x2%2)offset--;
			myth_assert(offset<0);
			if (q->top-q->base){
			  memmove(&q->ptr[q->base+offset],&q->ptr[q->base],sizeof(myth_thread_t)*(q->top-q->base));
			}
			q->top+=offset;q->base+=offset;
		}
		t=q->top;
		myth_wsqueue_lock_unlock(&q->lock);
	}
	//Do not need to extend of move.
	q->ptr[t]=th;
	myth_wsqueue_wbarrier();//Guarantee W-W dependency
	q->top=t+1;
#if defined USE_LOCK || defined USE_LOCK_PUSH
	myth_internal_lock_unlock(&q->m_lock);
#endif
	myth_queue_exit_operation(q);
}

#ifndef MYTH_QUEUE_FIFO
static inline myth_thread_t __attribute__((always_inline)) myth_queue_pop(myth_thread_queue_t q)
{
	myth_queue_enter_operation(q);

#ifdef QUICK_CHECK_ON_POP
	if (q->top <= q->base) {
	  return NULL;
	}
#endif

#if defined USE_LOCK || defined USE_LOCK_POP
	myth_internal_lock_lock(&q->m_lock);
#endif
	myth_thread_t ret;
	int top,base;
	top=q->top;
	top--;
	q->top=top;
	//Decrement and check top
	myth_wsqueue_rwbarrier();
	base=q->base;
	if (base+1<top){
		ret=q->ptr[top];
		//q->ptr[top]=NULL;
#if defined USE_LOCK || defined USE_LOCK_POP
		myth_internal_lock_unlock(&q->m_lock);
#endif
		myth_queue_exit_operation(q);
		return ret;
	}
	else{
		myth_wsqueue_lock_lock(&q->lock);
		if (q->base<=top){//OK
			ret=q->ptr[top];
			q->ptr[top]=NULL;
			myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_POP
			myth_internal_lock_unlock(&q->m_lock);
#endif
			myth_queue_exit_operation(q);
			return ret;
		}
		else{
			q->top=q->size/2;
			q->base=q->size/2;
			myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_POP
			myth_internal_lock_unlock(&q->m_lock);
#endif
			myth_queue_exit_operation(q);
			return NULL;
		}
		myth_unreachable();
	}
	myth_unreachable();
}
#else
static inline myth_thread_t myth_queue_take(myth_thread_queue_t q);
static inline myth_thread_t myth_queue_pop(myth_thread_queue_t q){
	return myth_queue_take(q);
}
#endif

//take/pass:Non-owner functions
static inline myth_thread_t myth_queue_take(myth_thread_queue_t q)
{
	myth_thread_t ret;
	int b,top;
#ifdef QUICK_CHECK_ON_STEAL
	if (q->top-q->base<=0){
		return NULL;
	}
#endif
#if defined USE_LOCK || defined USE_LOCK_TAKE
	myth_internal_lock_lock(&q->m_lock);
#endif
#ifdef TRY_LOCK_BEFORE_STEAL
	if (!myth_internal_lock_trylock(&q->lock)){
		myth_queue_exit_operation();
		return NULL;
	}
#else
	myth_wsqueue_lock_lock(&q->lock);
#endif
	//Increment base
	b=q->base;
	q->base=b+1;
	myth_wsqueue_rwbarrier();
	top=q->top;
	if (b<top){
		ret=q->ptr[b];
		//q->ptr[b]=NULL;
		myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_TAKE
		myth_internal_lock_unlock(&q->m_lock);
#endif
		return ret;
	}else{
		q->base=b;
		myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_TAKE
		myth_internal_lock_unlock(&q->m_lock);
#endif
		return NULL;
	}
	myth_unreachable();
}

static inline int myth_queue_trypass(myth_thread_queue_t q,myth_thread_t th)
{
#if defined USE_LOCK || defined USE_LOCK_TRYPASS
	myth_internal_lock_lock(&q->m_lock);
#endif
	int ret=1;
	if (!myth_wsqueue_lock_trylock(&q->lock))return 0;
	if (q->base==0){
		ret=0;
	}
	else{
		int b;
		b=q->base;
		q->ptr[b-1]=th;
		myth_wsqueue_wbarrier();
		q->base--;
	}
	myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_TRYPASS
	myth_internal_lock_unlock(&q->m_lock);
#endif
	return ret;
}

static inline void myth_queue_pass(myth_thread_queue_t q,myth_thread_t th)
{
	int ret;
	do{
		ret=myth_queue_trypass(q,th);
	}while(ret==0);
}


//put:Owner function: put a thread to the tail of the queue
static inline void myth_queue_put(myth_thread_queue_t q,myth_thread_t th)
{
	myth_queue_enter_operation(q);
#if defined USE_LOCK || defined USE_LOCK_PUSH
	myth_internal_lock_lock(&q->m_lock);
#endif
	myth_wsqueue_lock_lock(&q->lock);
	if (q->base==0){
		if (q->top==q->size){
			myth_assert(0);
			fprintf(stderr,"Fatal error:Runqueue overflow\n");
			abort();
			/*myth_thread_t *newptr;
			int newsize;
			newsize=q->size*2;
			newptr=myth_malloc(sizeof(myth_thread_t)*newsize);
			memcpy(newptr+,q->ptr+base,sizeof(myth_thread_t)*(q->top-q->base+1));
			q->top=;q->base=;
			myth_free(q->ptr);q->ptr=newptr;q->size=newsize;*/
		}
		else{
			int offset,offset_x2;
			offset_x2=q->size-(q->base+q->top);
			offset=offset_x2/2;if (offset_x2%2)offset--;
			myth_assert(offset<0);
			if (q->top-q->base){
			  memmove(&q->ptr[q->base+offset],&q->ptr[q->base],sizeof(myth_thread_t)*(q->top-q->base));
			}
			q->top+=offset;q->base+=offset;
		}
	}
	int b=q->base;
	if (b==0){myth_unreachable();}
	b--;
	q->ptr[b]=th;
	q->base=b;
	myth_wsqueue_lock_unlock(&q->lock);
#if defined USE_LOCK || defined USE_LOCK_PUSH
	myth_internal_lock_unlock(&q->m_lock);
#endif
	myth_queue_exit_operation(q);
}


#endif /* MYTH_QUEUE_FUNC_H_ */
