#ifndef OMP_DEFINES_H
#define OMP_DEFINES_H

#include "macros.h"

// this file prototypes the OMP calls supplied by the Qthreads implementation
//   the user is expected to use omp.h from amother implementation, but the
//   Qthreads build does not want to depend on having it as part of the build
//   process

// Wrappered OMP functions from omp.h
#ifndef _OMP_H_DEF
# define _OMP_H_DEF

# ifndef OMP_H
#  define OMP_H

Q_STARTCXX /* */

typedef struct omp_lock_t {
    void * _lk;
} omp_lock_t;

#include <stdint.h>

void    omp_set_nested(int val);
void    omp_set_num_threads(int x);
int     omp_get_num_threads(void);
int     omp_get_max_threads(void);
int     omp_get_thread_num(void);
int     omp_get_num_procs(void);
int     omp_in_parallel(void);
char    omp_in_final(void);
void    omp_set_dynamic(int val);
int     omp_get_dynamic(void);
void    omp_init_lock(void *pval);
void    omp_destroy_lock(void *pval);
void    omp_set_lock(void *pval);
void    omp_unset_lock(void *pval);
int     omp_test_lock(void *pval);
void    omp_init_nest_lock(void *pval);
void    omp_destroy_nest_lock(void *pval);
void    omp_set_nested(int val);
int64_t omp_get_nested(void);
void    omp_set_nest_lock(void *pval);
void    omp_unset_nest_lock(void *pval);
int     omp_test_nest_lock(void *pval);
double  omp_get_wtime(void);
double  omp_get_wtick(void);

Q_ENDCXX /* */

# endif // ifndef OMP_H
#endif // ifndef _OMP_H_DEF

#endif // ifndef OMP_DEFINES_H
/* vim:set expandtab: */
