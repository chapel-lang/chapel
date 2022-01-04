#ifndef QLOOP_HPP
#define QLOOP_HPP

#include "qloop.h"

template <typename T>
void qloop_cpp_wrapper(size_t startat,
                       size_t stopat,
                       void  *_arg)
{                                       /*{{{ */
    T *arg = (T *)_arg;

    (*arg)(startat,
           stopat);
}                                       /*}}} */

template <typename T>
void qt_loop(size_t   start,
             size_t   stop,
             const T &obj)
{                                       /*{{{ */
    qt_loop(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T &>(obj)));
}                                       /*}}} */

template <typename T>
void qt_loop_future(size_t   start,
                    size_t   stop,
                    const T &obj)
{   /*{{{*/
    qt_loop_future(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T &>(obj)));
} /*}}}*/

template <typename T>
void qt_loop_balance(size_t   start,
                     size_t   stop,
                     const T &obj)
{                                       /*{{{ */
    qt_loop_balance(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T &>(obj)));
}                                       /*}}} */

template <typename T>
void qt_loop_balance_simple(size_t   start,
                            size_t   stop,
                            const T &obj)
{                                       /*{{{ */
    qt_loop_balance_simple(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T &>(obj)));
}                                       /*}}} */

template <typename T>
void qt_loop_balance_future(size_t   start,
                            size_t   stop,
                            const T &obj)
{                                       /*{{{ */
    qt_loop_balance_future(start, stop, qloop_cpp_wrapper<T>,
                           &(const_cast<T &>(obj)));
}                                       /*}}} */

template <typename T>
void qloop_accum_cpp_wrapper(size_t startat,
                             size_t stopat,
                             void  *_arg,
                             void  *ret)
{                                       /*{{{ */
    T *arg = (T *)_arg;

    typename T::acctype tmp     = (*arg)(startat,
                                         stopat);
    *(typename T::acctype *)ret = tmp;
}                                       /*}}} */

template <typename T>
typename T::acctype qt_loopaccum_balance(size_t   start,
                                         size_t   stop,
                                         const T &obj)
{
    typename T::acctype accumulate(T::identity);
    qt_loopaccum_balance(start, stop, sizeof(typename T::acctype), &accumulate, qloop_accum_cpp_wrapper<T>, &(const_cast<T &>(obj)), (qt_accum_f)(T::accumulate));
    return accumulate;
}
#endif // ifndef QLOOP_HPP
/* vim:set expandtab: */
