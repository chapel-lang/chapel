// StarPU --- Runtime system for heterogeneous multicore architectures.
//
// Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique
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

/*
@seek@
identifier func;
expression E;
statement S1, S2;
position p;
identifier cuda_func =~ "^cuda";
@@
func(...)
{
...
E@p = cuda_func(...);
... when != if (!E) S1
    when != if (!E) S1 else S2
    when != if (E) S1
    when != !E
    when != E != cudaSuccess
    when != E == cudaSuccess
    when != STARPU_UNLIKELY(!E)
}

@fix@
expression seek.E;
position seek.p;
identifier seek.cuda_func;
@@
E@p = cuda_func(...);
+ if (STARPU_UNLIKELY(E != cudaSuccess))
+	STARPU_CUDA_REPORT_ERROR(E);
*/


@initialize:python depends on report || org@
from re import sub
msg = "Ignoring the return value of %s."
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@no_assignment@
identifier cuda_func =~ "^cuda";
position p;
@@
cuda_func@p(...);


@depends on no_assignment && context@
identifier no_assignment.cuda_func;
position no_assignment.p;
@@
* cuda_func@p(...);

@script:python depends on no_assignment && org@
p << no_assignment.p;
func << no_assignment.cuda_func;
@@
coccilib.org.print_todo(p[0], orgmsg % func)

@depends on no_assignment && patch@
identifier no_assignment.cuda_func;
position no_assignment.p;
@@
-cuda_func@p(
+cudaError_t err = cuda_func(
...);
+ if (STARPU_UNLIKELY(err != cudaSuccess))
+	STARPU_CUDA_REPORT_ERROR(err);

@script:python depends on no_assignment && report@
p << no_assignment.p;
func << no_assignment.cuda_func;
@@
coccilib.report.print_report(p[0], msg % func)
