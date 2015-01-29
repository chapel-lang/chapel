// chplrt.h
//
//################################################################################
//# Inclusion of this file is enforced by a grep command in Makefile.runtime.foot,
//# so it should only contain macro definitions (directly or indirectly) which we
//# wish to impose on the runtime implementation.
//#
//# Inclusion of files containing extern (variable and function) declarations is
//# not accepted usage.
//################################################################################
//

#ifndef _chplrt_H_
#define _chplrt_H_

#include "sys_basic.h"

#ifndef LAUNCHER

#define _noop(x)

#include "chpl-mem-warning-macros.h"

#define exit    dont_use_exit_use_chpl_exit_instead

#endif // LAUNCHER

#endif
