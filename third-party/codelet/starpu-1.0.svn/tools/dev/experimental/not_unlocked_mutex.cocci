/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 inria
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */
virtual context
virtual org 
virtual patch
virtual report

@initialize:python depends on report || org@
msg="The mutex \"%s\" is not unlocked when leaving \"%s\""
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@r@
expression E;
identifier func;
position p;
@@
func(...)
{
<...
_STARPU_PTHREAD_MUTEX_LOCK(E);
<... when != _STARPU_PTHREAD_MUTEX_UNLOCK(E)
 if (...)
{
... when != _STARPU_PTHREAD_MUTEX_UNLOCK(E)
return@p ...;
}
...>
_STARPU_PTHREAD_MUTEX_UNLOCK(E);
...>
}

@depends on r && context@
position r.p;
@@
* return@p ...;

@script:python depends on r && org@
ps << r.p;
f << r.func;
E << r.E;
@@
for p in ps:
	coccilib.org.print_todo(p, orgmsg % (E, f))


@depends on r && patch@
expression r.E;
position r.p;
@@
+ _STARPU_PTHREAD_MUTEX_UNLOCK(E);
return@p ...;

@script:python depends on r && report@
ps << r.p;
f << r.func;
E << r.E;
@@
for p in ps:
	coccilib.org.print_todo(p, msg % (E, f))

