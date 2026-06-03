/* Portions of this file are Copyright (c) 2025 Tactical Computing Labs, LLC;
 * see COPYING */

#ifndef TASKIMPL_H
#define TASKIMPL_H

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
#include <stdarg.h>
#define NEEDARMMAKECONTEXT
#define NEEDSWAPCONTEXT
#include "arm-ucontext.h"
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_RISCV)
#include <stdarg.h>
#define NEEDRISCVMAKECONTEXT
#define NEEDSWAPCONTEXT
#include "riscv-ucontext.h"
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
#include <stdarg.h>
#define NEEDARMA64MAKECONTEXT
#define NEEDSWAPCONTEXT
#include "arm-ucontext.h"
#else
#error This platform has no fastcontext support
#endif

#endif // ifndef TASKIMPL_H
/* vim:set expandtab: */
