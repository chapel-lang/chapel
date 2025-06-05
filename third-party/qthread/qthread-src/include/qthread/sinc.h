#ifndef QT_SINC_H
#define QT_SINC_H

#include <stdalign.h>

#include "macros.h"
#include "qthread.h"

Q_STARTCXX /* */

  typedef void (*qt_sinc_op_f)(void *tgt, void const *src);

typedef struct qt_opaque_sinc_s {
  alignas(QTHREAD_ALIGNMENT_ALIGNED_T) uint8_t opaque_data[24];
} qt_sinc_t;

void qt_sinc_init(qt_sinc_t *restrict sinc,
                  size_t sizeof_value,
                  void const *restrict initial_value,
                  qt_sinc_op_f op,
                  size_t expect);
qt_sinc_t *qt_sinc_create(size_t sizeof_value,
                          void const *initial_value,
                          qt_sinc_op_f op,
                          size_t expect);
void qt_sinc_reset(qt_sinc_t *sinc, size_t expect);
void qt_sinc_fini(qt_sinc_t *sinc);
void qt_sinc_destroy(qt_sinc_t *sinc);
void qt_sinc_expect(qt_sinc_t *sinc, size_t count);
void *qt_sinc_tmpdata(qt_sinc_t *sinc);
void qt_sinc_submit(qt_sinc_t *restrict sinc, void const *restrict value);
void qt_sinc_wait(qt_sinc_t *restrict sinc, void *restrict target);

Q_ENDCXX /* */

#endif // ifndef QT_SINC_H
  /* vim:set expandtab: */
