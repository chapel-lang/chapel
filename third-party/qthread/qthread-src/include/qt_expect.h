#ifndef QTHREAD_EXPECT_H
#define QTHREAD_EXPECT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef QTHREAD_EXPECT_OKAY
# define QTHREAD_EXPECT(x,y) __builtin_expect(!!(x),(y))
# define QTHREAD_LIKELY(x) __builtin_expect(!!(x), 1)
# define QTHREAD_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
# define QTHREAD_EXPECT(x,y) (x)
# define QTHREAD_LIKELY(x) (x)
# define QTHREAD_UNLIKELY(x) (x)
#endif

#endif
