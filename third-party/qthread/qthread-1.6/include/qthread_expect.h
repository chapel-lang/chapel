#ifndef QTHREAD_EXPECT_H
#define QTHREAD_EXPECT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef QTHREAD_EXPECT_OKAY
# define QTHREAD_EXPECT(x,y) __builtin_expect(!!(x),(y))
#else
# define QTHREAD_EXPECT(x,y) (x)
#endif

#endif
