#ifndef QTHREAD_TIMER
#define QTHREAD_TIMER

#include "macros.h"

Q_STARTCXX /* */

typedef struct qtimer_s *qtimer_t;

unsigned long qtimer_fastrand(void);
double        qtimer_wtime(void);
double        qtimer_res(void);

qtimer_t qtimer_create(void);
void     qtimer_destroy(qtimer_t);
void     qtimer_start(qtimer_t);
void     qtimer_stop(qtimer_t);
double   qtimer_secs(qtimer_t);

Q_ENDCXX /* */

#endif // ifndef QTHREAD_TIMER
/* vim:set expandtab: */
