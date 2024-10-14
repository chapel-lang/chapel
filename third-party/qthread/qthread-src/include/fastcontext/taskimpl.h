#ifndef TASKIMPL_H
#define TASKIMPL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "qthread/common.h"

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
#define NEEDX86MAKECONTEXT
#define NEEDSWAPCONTEXT
#include "386-ucontext.h"
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
#define NEEDX86MAKECONTEXT
#define NEEDSWAPCONTEXT
#define NEEDX86REGISTERARGS
#include "386-ucontext.h"
#elif ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) ||                         \
       (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64))
#define NEEDPOWERMAKECONTEXT
#define NEEDSWAPCONTEXT
#include "power-ucontext.h"
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM)
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#define NEEDARMMAKECONTEXT
#define NEEDSWAPCONTEXT
#include "arm-ucontext.h"
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#define NEEDARMA64MAKECONTEXT
#define NEEDSWAPCONTEXT
#include "arm-ucontext.h"
#else
#error This platform has no fastcontext support
#endif

#endif // ifndef TASKIMPL_H
/* vim:set expandtab: */
