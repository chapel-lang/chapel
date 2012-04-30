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

/*
 * It is a bad idea to write code such as :
 *
 * for (i = 0; i < foo(...); i++) { ... }
 *
 * Indeed, foo will be called every time we enter the loop. This would be better :
 *
 * unsigned int max = foo(...);
 * for (i = 0; i < max; i++) { ... }
 *
 */

virtual context
virtual org 
virtual patch
virtual report

@initialize:python depends on report || org@
msg="Function call in the termination condition of a for loop"
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@r@
type t;
identifier f;
identifier it; 
expression E1;
position p;
@@
t it;
...
for@p (it = E1; it < f(...); ...)
{
...
}

@depends on r && context@
identifier r.f;
identifier r.it;
expression r.E1;
@@
* for (it = E1; it < f(...); ...)
{
...
}

@script:python depends on r && org@
p << r.p;
@@
coccilib.org.print_todo(p[0], orgmsg)

@depends on r && patch@
expression r.E1, E2, E3;
identifier r.it;
position r.p;
@@
-for@p(it = E1; it < E3; E2) 
+max = E3;
+for(it = E1; it < max; E2)
{
...
}

@script:python depends on r && report@
p << r.p;
@@
coccilib.report.print_report(p[0], msg)
