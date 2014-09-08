#ifndef _QTHREAD_HPP_
#define _QTHREAD_HPP_

#define QTHREAD_HAVE_CXX_INTERFACE
#ifdef qthread_incr
# undef qthread_incr
#endif

#include <limits>

#include <qthread/qthread.h>
#include <qthread/syncvar.hpp>

template <bool> class OnlyTrue;
template <> class OnlyTrue<true>
{};
#define QTHREAD_STATIC_ASSERT(X) (void)sizeof(OnlyTrue<(bool)(X)>)
#define QTHREAD_CHECKSIZE(X)     QTHREAD_STATIC_ASSERT(sizeof(X) == sizeof(aligned_t))

#define QTHREAD_CHECKINTEGER(X)  QTHREAD_STATIC_ASSERT(std::numeric_limits<X>::is_integer)
#define QTHREAD_CHECKUNSIGNED(X) QTHREAD_STATIC_ASSERT(!std::numeric_limits<X>::is_signed)

template <typename T>
inline int qthread_feb_status(const T *const addr)
{
    return qthread_feb_status((aligned_t *)addr);
}

/**************************************************************************
* fill
**************************************************************************/
template <typename T>
inline int qthread_empty(const T *const dest)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_empty((aligned_t *)dest);
}

inline int qthread_empty(syncvar *const dest)
{
    return dest->empty();
}

/**************************************************************************
* fill
**************************************************************************/
template <typename T>
inline int qthread_fill(const T *const dest)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_fill((aligned_t *)dest);
}

inline int qthread_fill(syncvar *const dest)
{
    return dest->fill();
}

/**************************************************************************
* writeEF
**************************************************************************/
template < typename T > inline int qthread_writeEF(T *const       dest,
                                                   const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_writeEF((aligned_t *)dest,
                           (aligned_t *)src);
}

template < typename T > inline int qthread_writeEF(syncvar *const dest,
                                                   const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return dest->writeEF(src);
}

template < typename T > inline int qthread_writeEF(T *const dest,
                                                   const T  src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_writeEF_const((aligned_t *)dest,
                                 (aligned_t)src);
}

template < typename T > inline int qthread_writeEF(syncvar *const dest,
                                                   const T        src)
{
    QTHREAD_CHECKSIZE(T);
    return dest->writeEF((uint64_t)src);
}

/**************************************************************************
* writeF
**************************************************************************/
template <typename T>
inline int qthread_writeF(T *const       dest,
                          const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_writeF((aligned_t *)dest,
                          (aligned_t *)src);
}

template <typename T>
inline int qthread_writeF(syncvar *const dest,
                          const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return dest->writeF(*(uint64_t *)src);
}

template <typename T>
inline int qthread_writeF(T *const dest,
                          const T  src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_writeF((aligned_t *)dest,
                          (aligned_t *)&src);
}

template <typename T>
inline int qthread_writeF(syncvar *const dest,
                          const T        src)
{
    QTHREAD_CHECKSIZE(T);
    return dest->writeF(*(uint64_t *)&src);
}

/**************************************************************************
* readFF
**************************************************************************/
template <typename T>
inline int qthread_readFF(T *const       dest,
                          const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_readFF((aligned_t *)dest,
                          (aligned_t *)src);
}

template <typename T>
inline int qthread_readFF(T *const       dest,
                          syncvar *const src)
{
    QTHREAD_CHECKSIZE(T);
    return src->readFF(dest);
}

template <typename T>
inline T qthread_readFF(const T *const src)
{
    T tmp;

    QTHREAD_CHECKSIZE(T);
    src->readFE(tmp);
    return tmp;
}

/**************************************************************************
* readFE
**************************************************************************/
template <typename T>
inline int qthread_readFE(T *const       dest,
                          const T *const src)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_readFE((aligned_t *)dest,
                          (aligned_t *)src);
}

template <typename T>
inline int qthread_readFE(T *const       dest,
                          syncvar *const src)
{
    QTHREAD_CHECKSIZE(T);
    return src->readFE(dest);
}

template <typename T>
inline T qthread_readFE(const T *const src)
{
    T tmp;

    QTHREAD_CHECKSIZE(T);
    src->readFE(tmp);
    return tmp;
}

/**************************************************************************
* lock
**************************************************************************/
template <typename T>
inline int qthread_lock(const T *const a)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_lock((aligned_t *)a);
}

/**************************************************************************
* unlock
**************************************************************************/
template <typename T>
inline int qthread_unlock(const T *const a)
{
    QTHREAD_CHECKSIZE(T);
    return qthread_unlock((aligned_t *)a);
}

/**************************************************************************
* incr
**************************************************************************/
inline float qthread_incr(float      *operand,
                          const float incr)
{
    return qthread_fincr(operand, incr);
}

inline double qthread_incr(double      *operand,
                           const double incr)
{
    return qthread_dincr(operand, incr);
}

template <typename T, typename T2>
inline T qthread_incr(T       *operand,
                      const T2 incr)
{
    QTHREAD_STATIC_ASSERT(sizeof(T) == 4 || sizeof(T) == 8);
    QTHREAD_CHECKINTEGER(T);
    QTHREAD_CHECKINTEGER(T2);
    switch (sizeof(T)) {
        case 4:
            return qthread_incr32((uint32_t *)operand, incr);

        case 8:
            return qthread_incr64((uint64_t *)operand, incr);

        default:
            *(volatile int *)(0) = 0;
    }
    return T(0);                       // never hit - keep compiler happy
}

#endif // QTHREAD_HPP_
/* vim:set expandtab: */
