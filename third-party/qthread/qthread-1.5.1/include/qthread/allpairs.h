#ifndef QTHREAD_ALLPAIRS_H
#define QTHREAD_ALLPAIRS_H

#include <qthread/qthread.h>
#include <qthread/qarray.h>

Q_STARTCXX			       /* */
typedef void (*dist_f) (const void *unit1, const void *unit2);
typedef void (*dist_out_f) (const void *unit1, const void *unit2,
			    void *restrict outstore);

void qt_allpairs(const qarray * array1, const qarray * array2,
		 const dist_f distfunc);
void qt_allpairs_output(const qarray * array1, const qarray * array2,
			const dist_out_f distfunc,
			void *restrict * restrict output,
			const size_t outsize);

Q_ENDCXX			       /* */
#endif
