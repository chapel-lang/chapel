#ifndef QTHREAD_ALLPAIRS_H
#define QTHREAD_ALLPAIRS_H

#include "macros.h"
#include "qarray.h"

Q_STARTCXX                             /* */
typedef void (*dist_f)(void const *unit1, void const *unit2);
typedef void (*dist_out_f)(void const *unit1, void const *unit2,
                           void *restrict outstore);

void qt_allpairs(qarray const *array1,
                 qarray const *array2,
                 dist_f distfunc);
void qt_allpairs_output(qarray const *array1,
                        qarray const *array2,
                        dist_out_f distfunc,
                        void *restrict *restrict output,
                        size_t outsize);

Q_ENDCXX                               /* */
#endif // ifndef QTHREAD_ALLPAIRS_H
/* vim:set expandtab: */
