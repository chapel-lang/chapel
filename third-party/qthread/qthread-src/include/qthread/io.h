#ifndef QTHREAD_IO_H
#define QTHREAD_IO_H

#include <qthread/macros.h>

Q_STARTCXX
void qt_begin_blocking_action(void);
void qt_end_blocking_action(void);
Q_ENDCXX

#endif
/* vim:set expandtab: */
