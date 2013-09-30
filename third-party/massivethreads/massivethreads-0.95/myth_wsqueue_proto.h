#ifndef MYTH_WSQUEUE_PROTO_H_
#define MYTH_WSQUEUE_PROTO_H_

static inline void myth_queue_init(myth_thread_queue_t q);
static inline void myth_queue_fini(myth_thread_queue_t q);
static inline void myth_queue_clear(myth_thread_queue_t q);
static inline void myth_queue_push(myth_thread_queue_t q,struct myth_thread *th);
static inline struct myth_thread* myth_queue_pop(myth_thread_queue_t q);
static inline void myth_queue_put(myth_thread_queue_t q,myth_thread_t th);
static inline struct myth_thread* myth_queue_take(myth_thread_queue_t q);
static inline int myth_queue_trypass(myth_thread_queue_t q,struct myth_thread* th);
static inline void myth_queue_pass(myth_thread_queue_t q,struct myth_thread* th);

static inline void myth_queue_init_thread_data(myth_queue_data_t th);
static inline void myth_queue_fini_thread_data(myth_queue_data_t th);

static inline int myth_queue_is_operating(myth_thread_queue_t q);

#endif /* MYTH_QUEUE_PROTO_H_ */
