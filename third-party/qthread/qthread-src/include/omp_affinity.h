#ifndef OMP_AFFINITY_H
#define OMP_AFFINITY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QTHREAD_OMP_AFFINITY

// Stealing modes
#define QTHREAD_STEAL_ON_ANY_IDLE 0
#define QTHREAD_STEAL_ON_ALL_IDLE 1

#define OMP_NO_CHILD_TASK_AFFINITY -1

unsigned int qthread_shepherds_available (
    void);

void qthread_disable_stealing (
    void);

void qthread_disable_stealing_onshep (
    unsigned int shep);

void qthread_disable_stealing_all (
    void);

void qthread_enable_stealing (
    unsigned int stealing_mode);

void qthread_enable_stealing_on_shep (
    unsigned int shep,
    unsigned int stealing_mode);

void qthread_enable_stealing_all (
    unsigned int stealing_mode);

void omp_child_task_affinity (
    unsigned int shep);

void * omp_affinity_alloc(size_t bytes);
void * omp_affinity_alloc_onshep(size_t bytes, unsigned int shep);
void omp_affinity_mem_toshep(void * addr, size_t bytes, unsigned int shep);
void omp_affinity_free(void * ptr, size_t bytes);

#endif

#ifdef __cplusplus
}
#endif

#endif
