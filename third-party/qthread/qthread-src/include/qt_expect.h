#ifndef QTHREAD_EXPECT_H
#define QTHREAD_EXPECT_H

#define QTHREAD_EXPECT(x, y) __builtin_expect(!!(x), (y))
#define QTHREAD_LIKELY(x) __builtin_expect(!!(x), 1)
#define QTHREAD_UNLIKELY(x) __builtin_expect(!!(x), 0)

#endif
