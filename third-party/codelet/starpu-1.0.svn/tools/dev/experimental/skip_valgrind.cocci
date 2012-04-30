/*
 * StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012 INRIA
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
msg="Should you add STARPU_SKIP_IF_VALGRIND; at the beginning of this function ?"
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@find_codelet@
identifier a, b;
identifier func;
position p;
@@
void func@p(void *a[], void *b)
{
...
}

@is_empty_codelet@
identifier find_codelet.a, find_codelet.b;
identifier find_codelet.func;
position find_codelet.p;
@@
void func@p(void *a[], void *b)
{
}

@is_already_ok@
identifier find_codelet.a, find_codelet.b;
identifier find_codelet.func;
position find_codelet.p;
@@
void func@p(void *a[], void *b)
{
	STARPU_SKIP_IF_VALGRIND;
...
}


@depends on find_codelet && !is_empty_codelet && !is_already_ok && context@
identifier find_codelet.a, find_codelet.b;
identifier find_codelet.func;
position find_codelet.p;
@@
*void func@p(void *a[], void *b) 
{
...
}

@script:python depends on find_codelet && !is_empty_codelet && !is_already_ok && org@
p << find_codelet.p;
@@
coccilib.org.print_todo(p[0], orgmsg)

@depends on find_codelet && !is_empty_codelet && !is_already_ok && patch@
identifier find_codelet.a, find_codelet.b;
identifier find_codelet.func;
position find_codelet.p;
@@
void func@p(void *a[], void *b) 
{
+STARPU_SKIP_IF_VALGRIND;
+
...
}

@script:python depends on find_codelet && !is_empty_codelet && !is_already_ok && report@
p << find_codelet.p;
@@
coccilib.report.print_report(p[0], msg)
