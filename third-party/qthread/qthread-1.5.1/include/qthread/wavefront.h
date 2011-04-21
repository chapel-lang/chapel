#ifndef QTHREAD_WAVEFRONT_H
#define QTHREAD_WAVEFRONT_H

#include <qthread/qarray.h>

Q_STARTCXX			       /* */
typedef void (*wave_comp_f) (const void *restrict left,
			     const void *restrict leftdown,
			     const void *restrict down, void *restrict out);
typedef void (*wave_f) (qarray * restrict left, qarray * restrict below,
			void **R);

typedef struct qt_wavefront_lattice_s qt_wavefront_lattice;

qt_wavefront_lattice *qt_wavefront(qarray * restrict const left,
				   qarray * restrict const below,
				   wave_comp_f func);
void qt_wavefront_print_lattice(const qt_wavefront_lattice * const L);
void qt_wavefront_destroy_lattice(qt_wavefront_lattice * const L);

void qt_basic_wavefront(int *restrict const *const R, size_t cols,
			size_t rows, wave_comp_f func);

Q_ENDCXX			       /* */
#endif
