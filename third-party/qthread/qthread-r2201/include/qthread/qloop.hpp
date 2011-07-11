#ifndef QLOOP_HPP
#define QLOOP_HPP

#include <qthread/qloop.h>

template <typename T>
void qloop_cpp_wrapper(size_t startat,
                       size_t stopat,
                       void  *_arg)
{                                       /*{{{ */
    T *arg = (T *)_arg;

    (*arg)(startat, stopat);
}                                       /*}}} */

template <typename T>
void qt_loop(const size_t start,
             const size_t stop,
             const T&     obj)
{                                       /*{{{ */
    qt_loop(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T&>(obj)));
}                                       /*}}} */

template <typename T>
void qt_loop_future(const size_t start,
                    const size_t stop,
                    const T&     obj)
{   /*{{{*/
    qt_loop_future(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T&>(obj)));
} /*}}}*/

template <typename T>
void qt_loop_balance(const size_t start,
                     const size_t stop,
                     const T&     obj)
{                                       /*{{{ */
    qt_loop_balance(start, stop, qloop_cpp_wrapper<T>, &(const_cast<T&>(obj)));
}                                       /*}}} */

template <typename T>
void qt_loop_balance_future(const size_t start,
                            const size_t stop,
                            const T&     obj)
{                                       /*{{{ */
    qt_loop_balance_future(start, stop, qloop_cpp_wrapper<T>,
                           &(const_cast<T&>(obj)));
}                                       /*}}} */

#endif // ifndef QLOOP_HPP
/* vim:set expandtab: */
