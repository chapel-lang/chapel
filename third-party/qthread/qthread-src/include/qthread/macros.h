#ifndef QTHREAD_MACROS_H
#define QTHREAD_MACROS_H

#ifdef __cplusplus
# define Q_STARTCXX extern "C" {
# define Q_ENDCXX   }
#else
# define Q_STARTCXX
# define Q_ENDCXX
#endif

#endif
