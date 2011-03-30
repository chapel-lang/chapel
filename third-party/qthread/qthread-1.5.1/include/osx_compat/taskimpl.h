/* Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */
#ifndef TASKIMPL_H
#define TASKIMPL_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* HAS_UCONTEXT affects the include files on some systems (I think) */
#if defined(__linux__) || defined(__NetBSD__) || defined(__FreeBSD__) || (defined(__SVR4) && defined (__sun))
#define HAS_UCONTEXT 1
#endif

#if defined(__APPLE__)
#if !defined(_BSD_PPC_SETJMP_H_)
#include <setjmp.h>
#define HAS_UCONTEXT 1
#endif
#endif

#if defined(__tile__)
#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif
#include <stddef.h>
#define NEEDTILEMAKECONTEXT
#define NEEDSWAPCONTEXT
#endif

#if defined(__FreeBSD__) ||  defined(__APPLE__) || defined(__linux__) || defined(__CYGWIN32__)

#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif
#include <errno.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifndef __tile__
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif
# ifdef HAVE_SCHED_H
#  include <sched.h>
# endif
# ifdef HAVE_SIGNAL_H
#  include <signal.h>
# endif
#endif
#ifdef HAVE_SYS_UTSNAME_H
# include <sys/utsname.h>
#endif
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#ifndef __tile__
# ifdef HAVE_SYS_UCONTEXT_H
#  include <sys/ucontext.h>
# endif
#endif

#if defined(__FreeBSD__) && __FreeBSD__ < 5
extern	int		getmcontext(mcontext_t*);
extern	void		setmcontext(mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
extern	int		swapcontext(ucontext_t*, ucontext_t*);
extern	void		makecontext(ucontext_t*, void(*)(), int, ...);
#endif

#ifdef __CYGWIN32__
# define NEEDX86MAKECONTEXT
# define NEEDSWAPCONTEXT
#endif

#if (defined(__APPLE__) || defined(__linux__)) && (defined(__i386__) || defined(__x86_64__))
# define NEEDX86MAKECONTEXT
# define NEEDSWAPCONTEXT
# if defined(__x86_64__)
#  define NEEDX86REGISTERARGS
# endif
#endif

#if (defined(__APPLE__) || defined(__linux__)) && (defined(__ppc__) || defined(__ppc64__))
#define NEEDPOWERMAKECONTEXT
#define NEEDSWAPCONTEXT
#endif

#if defined(__FreeBSD__) && defined(__i386__) && __FreeBSD__ < 5
#define NEEDX86MAKECONTEXT
#define NEEDSWAPCONTEXT
#endif

#if defined(__APPLE__) || defined(__linux__) || defined(__CYGWIN32__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined(__i386__) || defined(__x86_64__)
#		include "386-ucontext.h"
#       elif defined(__tile__)
#		include "tile-ucontext.h"
#	else
#		include "power-ucontext.h"
#	endif
#endif

#if defined(__OpenBSD__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined(__i386__) || defined(__x86_64__)
#		include "386-ucontext.h"
#	else
#		include "power-ucontext.h"
#	endif
extern pid_t rfork_thread(int, void*, int(*)(void*), void*);
#endif

#if 0 &&  defined(__sun__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	include "sparc-ucontext.h"
#endif

#if defined(__arm__)
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#endif
#endif

