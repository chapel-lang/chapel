// StarPU --- Runtime system for heterogeneous multicore architectures.
//
// Copyright (C) 2011-2012 inria
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

//
// General stuff for org and report modes.
//
@initialize:python depends on report || org@
d = { 'abort':'STARPU_ABORT', 'assert':'STARPU_ASSERT'}
msg = "Please use %s rather than %s."
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@r@
identifier f =~ "abort|assert";
position p;
@@
f@p(...);

@min@
expression E1,E2;
identifier i;
position p;
@@
(
return@p E1<E2?E1:E2;
|
i =@p  E1<E2?E1:E2
)

@max@
expression E1, E2;
identifier i;
position p;
@@
(
return@p E1>E2?E1:E2;
|
i =@p E1>E2?E1:E2
)

//
// Context mode.
//
@depends on context@
@@
*	abort();


@depends on context@
@@
*	assert(...);


@depends on context@
identifier i;
expression E1, E2;
@@
(
* 	return E1<E2?E1:E2;
|
*	i =  E1<E2?E1:E2            // No semi-colon at the end, so that it
|
*	return E1>E2?E1:E2;
|
*	i = E1>E2?E1:E2             // No semi-colon at the end, so that it
)

//
// Org mode.
//

@script:python depends on r && org@
p << r.p;
f << r.f;
@@
coccilib.org.print_todo(p[0], orgmsg % (d[str(f)], f))

@script:python depends on min && org@
p << min.p;
@@
coccilib.org.print_todo(p[0], "Please use STARPU_MIN")

@script:python depends on max && org@
p << max.p;
@@
coccilib.org.print_todo(p[0], "Please use STARPU_MAX")


//
// Patch mode.
//
@depends on patch@
@@
-	abort();
+	STARPU_ABORT();

@depends on patch@
@@
-	assert(
+	STARPU_ASSERT(
...)

@depends on patch@
identifier i;
expression E1, E2;
@@
(
- 	return E1<E2?E1:E2;
+ 	return STARPU_MIN(E1, E2);
|
-	i =  E1<E2?E1:E2            // No semi-colon at the end, so that it
+	i = STARPU_MIN(E1, E2)      // matches both "i = ..." and "t i = ..."
|
-	return E1>E2?E1:E2;
+	return STAPU_MAX(E1, E2);
|
-	i = E1>E2?E1:E2             // No semi-colon at the end, so that it
+	i = STARPU_MAX(E1, E2)      // matches both "i = ..." and "t i = ..."
)

//
// Report mode.
//
@script:python depends on r && report@
p << r.p;
f << r.f;
@@
coccilib.report.print_report(p[0], msg % (d[str(f)], f))

@script:python depends on min && report@
p << min.p;
@@
coccilib.report.print_report(p[0], "Please use STARPU_MIN")

@script:python depends on max && report@
p << max.p;
@@
coccilib.report.print_report(p[0], "Please use STARPU_MAX")
