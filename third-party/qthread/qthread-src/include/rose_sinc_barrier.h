#ifndef QT_SINC_BARRIER_H
#define QT_SINC_BARRIER_H

#include <qthread/sinc.h>

typedef struct qt_sinc_barrier_s {
  uint64_t count;
  qt_sinc_t *sinc_1;
  qt_sinc_t *sinc_2;
  qt_sinc_t *sinc_3;
}qt_sinc_barrier_t;

/* function calls for sinc based barrier -- allows barriers threads to be throttled within barrier regions - akp*/
void API_FUNC qt_sinc_barrier_enter(qt_sinc_barrier_t *b);
void qt_sinc_barrier_init(qt_sinc_barrier_t *restrict  barrier,
			  size_t               expect);
void qt_sinc_barrier_destroy(qt_sinc_barrier_t *restrict  barrier);
void qt_sinc_barrier_change(qt_sinc_barrier_t *restrict  barrier,
			    size_t               expect);

extern qt_sinc_barrier_t *sinc_barrier;  /* global barrier declaration -- may be dead now that nesting works - akp */ 

#endif
