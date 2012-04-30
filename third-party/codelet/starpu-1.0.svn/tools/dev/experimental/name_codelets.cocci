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
 * Codelets have a "name" attribute that is really useful for debugging
 * purposes. Lots of codelets do not use this attribute though, mostly because
 * they were written when it was not available.
 *
 * This semantic patch tries to add a "name" attribute when it is necessary.
 * The chosen name is the name of the codelet variable : it should be good
 * enough for debugging.
 */
virtual context
virtual org
virtual patch
virtual report

@initialize:python depends on org || report@
msg = "Warning: %s has no attribute name"
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@found@
identifier cl;
position p;
@@
struct starpu_codelet cl@p = {
...
};

@named depends on found@
position found.p;
identifier found.cl;
expression E;
@@
struct starpu_codelet cl@p = {
    .name = E,
};

// Context mode.
@depends on found && !named && context@
identifier found.cl;
position found.p;
@@
*struct starpu_codelet cl@p = {...};

// Org mode.
@script:python depends on found && !named && org@
cl << found.cl;
p << found.p;
@@
coccilib.org.print_todo(p[0], orgmsg % cl)

// Patch mode.
@script:python stringify depends on found && !named && patch@
cl_name << found.cl;
guess;
@@
coccinelle.guess = '"' + str(cl_name) + '"'

@depends on found && !named && patch@
position found.p;
identifier found.cl;
identifier stringify.guess;
@@
struct starpu_codelet cl@p = {
+.name = guess,
};

// Report mode.
@script:python depends on found && !named && report@
cl << found.cl;
p << found.p;
@@
coccilib.report.print_report(p[0], msg % cl)
