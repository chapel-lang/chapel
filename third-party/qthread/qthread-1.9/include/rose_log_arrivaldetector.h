#ifndef ROSE_LOG_ARRIVALDETECTOR_H
#define ROSE_LOG_ARRIVALDETECTOR_H

/* return 1 for first thread   0 for all others -- self cleaning */
/*   so that after everybody arrives can be use again            */
int64_t qt_global_arrive_first(const qthread_shepherd_id_t shep);
void qtar_resize(size_t s); /* reset the size of the psuedo barrier */
aligned_t  qtar_size();     /* how big is current psuedo barrier */
#endif
