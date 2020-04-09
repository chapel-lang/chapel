#ifndef QT_SUBSYSTEMS_H
#define QT_SUBSYSTEMS_H

#include "qt_visibility.h"

void INTERNAL qthread_internal_cleanup(void (*function)(void));
void INTERNAL qthread_internal_cleanup_early(void (*function)(void));
void INTERNAL qthread_internal_cleanup_late(void (*function)(void));

#endif
