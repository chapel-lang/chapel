#ifndef QT_ATOMIC_WAIT_H
#define QT_ATOMIC_WAIT_H

#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>

#include "qt_asserts.h"
#include "qt_os.h"

// This header defines a cross-platform futex-like API.
// It's somewhat like atomic_wait in c++20, however these constructs are
// guaranteed to use the appropriate OS APIs directly. It's also somewhat like
// Rust's https://github.com/m-ou-se/atomic-wait, except this is just a bunch of
// C macros and it supports a few more OSs.

// Linux only has 32-bit futexes so that's the only size that's possible to
// standardize.
#define qt_atomic_wait_t _Atomic uint32_t
#define qt_atomic_wait_empty 0u
#define qt_atomic_wait_full UINT32_MAX
#define qt_atomic_wait_set_empty(a)                                            \
  atomic_store_explicit((a), 0u, memory_order_relaxed)
#define qt_atomic_wait_set_full(a)                                             \
  atomic_store_explicit((a), UINT32_MAX, memory_order_relaxed)
#define qt_atomic_wait_load(a) atomic_load_explicit((a), memory_order_relaxed)
#define qt_atomic_wait_store(a, v)                                             \
  atomic_store_explicit((a), v, memory_order_relaxed)

// Futex-like atomic wait functionality that's guaranteed to use
// the appropriate OS thread pausing functionality (e.g. futex).
// Due to constraints between the various operating systems,
// only 32-bit integers are supported.

#ifdef QTHREADS_LINUX

// Use Linux futexes

#include <errno.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef NDEBUG
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    long status = syscall(SYS_futex,                                           \
                          (a),                                                 \
                          FUTEX_WAIT | FUTEX_PRIVATE_FLAG,                     \
                          (expected),                                          \
                          NULL,                                                \
                          NULL,                                                \
                          0u);                                                 \
    /* EAGAIN means the value already changed so no sleep was necessary. */    \
    assert(!status || (status == -1 && errno == EAGAIN));                      \
  } while (0)
#else
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    syscall(SYS_futex,                                                         \
            (a),                                                               \
            FUTEX_WAIT | FUTEX_PRIVATE_FLAG,                                   \
            (expected),                                                        \
            NULL,                                                              \
            NULL,                                                              \
            0u);                                                               \
  } while (0)
#endif

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    syscall(SYS_futex,                                                         \
            (a),                                                               \
            FUTEX_WAKE | FUTEX_PRIVATE_FLAG,                                   \
            UINT32_MAX,                                                        \
            NULL,                                                              \
            NULL,                                                              \
            0u);                                                               \
  } while (0)

#define qt_wake_one(a)                                                         \
  do {                                                                         \
    syscall(                                                                   \
      SYS_futex, (a), FUTEX_WAKE | FUTEX_PRIVATE_FLAG, 1u, NULL, NULL, 0u);    \
  } while (0)

#elif defined(QTHREADS_APPLE)
// use __ulock_wait and __ulock_wake
// NOTE! This isn't technically a stable API even though they export the
// symbols, so watch for changes in OSX releases later. libc++ relies on them
// though so changes seem very unlikely.

// Can't directly include apple's sys/ulock.h but we only need these symbols.
// They are exported from libSystem though, which also provides their libc.
// Given that, presumably we're already linking to them.
// See
// https://github.com/apple/darwin-xnu/blob/2ff845c2e033bd0ff64b5b6aa6063a1f8f65aa32/bsd/sys/ulock.h#L64-L68
extern int
__ulock_wait(uint32_t operation, void *addr, uint64_t value, uint32_t timeout);
extern int __ulock_wake(uint32_t operation, void *addr, uint64_t wake_value);

// Corresponding operation codes:
// See
// https://github.com/apple/darwin-xnu/blob/2ff845c2e033bd0ff64b5b6aa6063a1f8f65aa32/bsd/sys/ulock.h#L72-L137
#define UL_COMPARE_AND_WAIT 1
#define ULF_WAKE_ALL 0x00000100
#define ULF_NO_ERRNO 0x01000000

// Note: no need to check for the case where a wait operation
// wakes because the flag was already changed.
// In that case the return value ends up being the same.
// See
// https://github.com/apple/darwin-xnu/blob/2ff845c2e033bd0ff64b5b6aa6063a1f8f65aa32/osfmk/kern/waitq.c#L2819-L2820
// See also
// https://github.com/apple/darwin-xnu/blob/2ff845c2e033bd0ff64b5b6aa6063a1f8f65aa32/bsd/kern/sys_ulock.c#L424
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    qassert(                                                                   \
      __ulock_wait(UL_COMPARE_AND_WAIT | ULF_NO_ERRNO, (a), (expected), 0ull), \
      0);                                                                      \
  } while (0)

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    qassert(__ulock_wake(UL_COMPARE_AND_WAIT | ULF_WAKE_ALL | ULF_NO_ERRNO,    \
                         (a),                                                  \
                         0ull) >= 0,                                           \
            1);                                                                \
  } while (0)

#define qt_wake_one(a)                                                         \
  do { __ulock_wake(UL_COMPARE_AND_WAIT | ULF_NO_ERRNO, (a), 0ull); } while (0)

#elif defined(QTHREADS_FREEBSD)
// use _umtx_op
#include <sys/types.h>
#include <sys/umtx.h>

#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    qassert(_umtx_op((a), UMTX_OP_WAIT_UINT_PRIVATE, (expected), NULL, NULL),  \
            0);                                                                \
  } while (0)

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    /* Docs say use INT32_MAX to specify all. */                               \
    qassert(                                                                   \
      _umtx_op(                                                                \
        (a), UMTX_OP_WAKE_PRIVATE, (unsigned long)INT32_MAX, NULL, NULL),      \
      0);                                                                      \
  } while (0)

#define qt_wake_one(a)                                                         \
  do {                                                                         \
    qassert(_umtx_op((a), UMTX_OP_WAKE_PRIVATE, 1ul, NULL, NULL), 0);          \
  } while (0)

#elif defined(QTHREADS_OPENBSD)
// use futex syscall wrapper they provide: https://man.openbsd.org/futex
#include <errno.h>
#include <sys/futex.h>
#include <sys/time.h>

#ifndef NDEBUG
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    int status =                                                               \
      futex((a), FUTEX_WAIT | FUTEX_PRIVATE_FLAG, (expected), NULL, NULL);     \
    assert(!status || (status == -1 && errno == EAGAIN));
}
while (0)
#else
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    futex((a), FUTEX_WAIT | FUTEX_PRIVATE_FLAG, (expected), NULL, NULL);       \
  } while (0)
#endif

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    /* For whatever reason they used a signed integer for the val parameter so \
     * use INT32_MAX.*/                                                        \
    futex((a), FUTEX_WAKE | FUTEX_PRIVATE_FLAG, INT32_MAX, NULL, NULL);        \
  } while (0)

#define qt_wake_one(a)                                                         \
  do { futex((a), FUTEX_WAKE | FUTEX_PRIVATE_FLAG, 1, NULL, NULL); } while (0)

#elif defined(QTHREADS_NETBSD)
// use SYS___futex syscall
#include <errno.h>
#include <sys/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>

#ifndef NDEBUG
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    int status = syscall(SYS___futex,                                          \
                         (a),                                                  \
                         FUTEX_WAIT | FUTEX_PRIVATE_FLAG,                      \
                         (expected),                                           \
                         NULL,                                                 \
                         NULL);                                                \
    assert(!status || status == -1 && errno == EAGAIN);                        \
  } while (0)
#else
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    syscall(SYS___futex,                                                       \
            (a),                                                               \
            FUTEX_WAIT | FUTEX_PRIVATE_FLAG,                                   \
            (expected),                                                        \
            NULL,                                                              \
            NULL);                                                             \
    while (0)
#endif

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    syscall(SYS___futex,                                                       \
            (a),                                                               \
            FUTEX_WAKE | FUTEX_PRIVATE_FLAG,                                   \
            UINT32_MAX,                                                        \
            NULL,                                                              \
            NULL);                                                             \
  } while (0)

#define qt_wake_one(a)                                                         \
  do {                                                                         \
    syscall(                                                                   \
      SYS___futex, (a), FUTEX_WAKE | FUTEX_PRIVATE_FLAG, 1u, NULL, NULL);      \
  } while (0)

#elif defined(QTHREADS_DRAGONFLYBSD)
// use umtx_sleep and umtx_wakeup
#include <errno.h>
#include <unistd.h>
#ifndef NDEBUG
#define qt_wait_on_address(a, expected)                                        \
  do {                                                                         \
    int status = umtx_sleep((a), (expected), 0);                               \
    assert(!status || (status == -1 && errno == EBUSY));                       \
  } while (0)
#else
#define qt_wait_on_address(a, expected)                                        \
  do { umtx_sleep((a), (expected), 0); } while (0)
#endif

#define qt_wake_all(a)                                                         \
  do {                                                                         \
    /* The interface uses signed integers, so use INT32_MAX here */            \
    qassert(umtx_wakeup((a), INT32_MAX), 0);                                   \
  } while (0)

#define qt_wake_one(a)                                                         \
  do { qassert(umtx_wakeup((a), 1), 0); } while (0)

#elif defined(QTHREADS_WINDOWS)
// use WaitOnAddress/WakeByAddressSingle/WakeByAddressAll
#include <synchapi.h>
#define qt_wait_on_address(a, expected)                                        \
  do { qassert(WaitOnAddress((a), (expected), 4, INFINITE), TRUE); } while (0)

#define qt_wake_all(a)                                                         \
  do { WakeByAddressAll(a); } while (0)

#define qt_wake_one(a)                                                         \
  do { WakeByAddressSingle(a); } while (0)

#elif defined(__sun)
// Solaris supposedly provides something futex-like via "user-level adaptive
// spin mutexes".
// TODO: implement that
#error "futex equivalent not implemented for solaris."

#else
#error "no known futex equivalent for current OS"
#endif

#endif
