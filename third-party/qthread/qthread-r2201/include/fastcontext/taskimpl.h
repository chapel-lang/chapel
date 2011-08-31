/* Portions of this file copyright (c) 2005-2006 Russ Cox, MIT; see COPYING */
#ifndef TASKIMPL_H
#define TASKIMPL_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if defined(__tile__)
# ifdef HAVE_STDARG_H
#  include <stdarg.h>
# endif
# include <stddef.h>
# define NEEDTILEMAKECONTEXT
# define NEEDSWAPCONTEXT
#endif

#if (defined(__i386__) || defined(__x86_64__))
# define NEEDX86MAKECONTEXT
# define NEEDSWAPCONTEXT
# if defined(__x86_64__)
#  define NEEDX86REGISTERARGS
# endif
#endif

#if (defined(__ppc__) || defined(__ppc64__))
# define NEEDPOWERMAKECONTEXT
# define NEEDSWAPCONTEXT
#endif

#if defined(__APPLE__) || defined(__linux__) || defined(__CYGWIN32__)
# if defined(__i386__) || defined(__x86_64__)
#  include "386-ucontext.h"
# elif defined(__tile__)
#  include "tile-ucontext.h"
# else
#  include "power-ucontext.h"
# endif
#endif

#if 0 && defined(__sun__)
# include "sparc-ucontext.h"
#endif

#endif // ifndef TASKIMPL_H
/* vim:set expandtab: */
