#ifndef LOOP_DEFINES_H
#define LOOP_DEFINES_H

#include <cstring>

#ifdef USING_QTHREADS
# include <qthread/qthread.h>
# include <qthread/qloop.hpp>
# define LOOP_BEGIN(x, y, b, e) qt_loop_balance((x), (y), [&](size_t b, size_t e) {
# define LOOP_END() })
#else
# define LOOP_BEGIN(x, y, b, e) const size_t b = (x); const size_t e = (y);
# define LOOP_END()
#endif

#endif
