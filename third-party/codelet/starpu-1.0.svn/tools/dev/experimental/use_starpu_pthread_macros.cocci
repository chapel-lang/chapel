// StarPU --- Runtime system for heterogeneous multicore architectures.
//
// Copyright (C) 2011-2012 inria
// Copyright (C) 2011 Centre National de la Recherche Scientifique
//
// StarPU is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// StarPU is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// See the GNU Lesser General Public License in COPYING.LGPL for more details.

virtual context
virtual org
virtual patch
virtual report

@initialize:python depends on report || org@
d = {
'pthread_mutex_init'      : '_STARPU_PTHREAD_MUTEX_INIT',
'pthread_mutex_lock'      : '_STARPU_PTHREAD_MUTEX_LOCK',
'pthread_mutex_unlock'    : '_STARPU_PTHREAD_MUTEX_UNLOCK',
'pthread_mutex_destroy'   : '_STARPU_PTHREAD_MUTEX_DESTROY',
'pthread_rwlock_init'     : '_STARPU_PTHREAD_RWLOCK_INIT',
'pthread_rwlock_rdlock'   : '_STARPU_PTHREAD_RWLOCK_RDLOCK',
'pthread_rwlock_wrlock'   : '_STARPU_PTHREAD_RWLOCK_WRLOCK',
'pthread_rwlock_unlock'   : '_STARPU_PTHREAD_RWLOCK_UNLOCK',
'pthread_rwlock_destroy'  : '_STARPU_PTHREAD_RWLOCK_DESTROY',
'pthread_cond_init'       : '_STARPU_PTHREAD_COND_INIT',
'pthread_cond_signal'     : '_STARPU_PTHREAD_COND_SIGNAL',
'pthread_cond_broadcast'  : '_STARPU_PTHREAD_COND_BROADCAST',
'pthread_cond_wait'       : '_STARPU_PTHREAD_COND_WAIT',
'pthread_cond_destroy'    : '_STARPU_PTHREAD_COND_DESTROY',
'pthread_barrier_init'    : '_STARPU_PTHREAD_BARRIER_INIT',
'pthread_barrier_wait'    : '_STARPU_PTHREAD_BARRIER_WAIT',
'pthread_barrier_destroy' : '_STARPU_PTHREAD_BARRIER_DESTROY',
'pthread_spin_destroy'    : '_STARPU_PTHREAD_SPIN_DESTROY',
'pthread_spin_lock'       : '_STARPU_PTHREAD_SPIN_LOCK',
'pthread_spin_unlock'     : '_STARPU_PTHREAD_SPIN_UNLOCK'
}
msg = "Use %s instead of %s."
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@r@
identifier f =~ "^pthread_";
position p;
@@
f@p(...)

//
// Context mode.
//
@depends on context@
identifier f =~ "^pthread_";
@@
* f(...)

//
// Org mode.
//
@script:python depends on r && org@
p << r.p;
f << r.f;
@@
if str(f) in d.keys():
	coccilib.org.print_todo(p[0], orgmsg % (d[str(f)], f))
else:
	coccilib.org.print_todo(p[0], "Shouldn't =%s= be wrapped in a macro ?" % str(f))


//
// Patch mode.
//
@pthread_mutex_ depends on patch@
expression E1, E2;
@@
(
- pthread_mutex_init(E1, E2);
+ _STARPU_PTHREAD_MUTEX_INIT(E1, E2);
|
- pthread_mutex_lock(E1);
+ _STARPU_PTHREAD_MUTEX_LOCK(E1);
|
- pthread_mutex_unlock(E1);
+ _STARPU_PTHREAD_MUTEX_UNLOCK(E1);
|
- pthread_mutex_destroy(E1);
+ _STARPU_PTHREAD_MUTEX_DESTROY(E1);
)


@pthread_rwlock_ depends on patch@
expression E;
@@
(
- pthread_rwlock_init(E);
+ _STARPU_PTHREAD_RWLOCK_INIT(E);
|
- pthread_rwlock_rdlock(E);
+ _STARPU_PTHREAD_RWLOCK_RDLOCK(E);
|
- pthread_rwlock_wrlock(E);
+ _STARPU_PTHREAD_RWLOCK_WRLOCK(E);
|
- pthread_rwlock_unlock(E);
+ _STARPU_PTHREAD_RWLOCK_UNLOCK(E);
|
- pthread_rwlock_destroy(E);
+ _STARPU_PTHREAD_RWLOCK_DESTROY(E);
)


@pthread_cond_ depends on patch@
expression E1, E2;
@@
(
- pthread_cond_init(E1, E2);
+ _STARPU_PTHREAD_COND_INIT(E1, E2);
|
- pthread_cond_signal(E1);
+ _STARPU_PTHREAD_COND_SIGNAL(E1);
|
- pthread_cond_broadcast(E1);
+ _STARPU_PTHREAD_COND_BROADCAST(E1);
|
- pthread_cond_wait(E1, E2);
+ _STARPU_PTHREAD_COND_WAIT(E1, E2);
|
- pthread_cond_destroy(E1);
+ _STARPU_PTHREAD_COND_DESTROY(E1);
)


@pthread_barrier_ depends on patch@
expression E1, E2, E3;
@@
(
- pthread_barrier_init(E1, E2, E3);
+ _STARPU_PTHREAD_BARRIER_INIT(E1, E2, E3);
|
- pthread_barrier_wait(E1);
+ _STARPU_PTHREAD_BARRIER_WAIT(E1);
|
- pthread_barrier_destroy(E1);
+ _STARPU_PTHREAD_BARRIER_DESTROY(E1);
)

@pthread_spin_ depends on patch@
expression E1;
@@
(
- pthread_spin_destroy(E1);
+ _STARPU_PTHREAD_SPIN_DESTROY(E1);
|
- pthread_spin_lock(E1);
+ _STARPU_PTHREAD_SPIN_LOCK(E1);
|
- pthread_spin_unlock(E1);
+ _STARPU_PTHREAD_SPIN_UNLOCK(E1);
)

//
// Report mode.
//
@script:python depends on r && report@
p << r.p;
f << r.f;
@@
if str(f) in d.keys():
	coccilib.report.print_report(p[0], msg % (d[str(f)], f))
else:
	coccilib.report.print_report(p[0], "Shouldn't %s be wrapped in a macro ?" % str(f))
