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
 * When the submission of a task fails, StarPU cannot destroy the task, even if
 * the destroy flag is set. So we have to destroy it ourselves while handling
 * the error.
 *
 * TODO: match if statments without braces.
 */

virtual context
virtual org
virtual patch
virtual report

@initialize:python depends on org || report@
msg = "Warning: in %s(): "
msg+= "\"%s\" should probably be destroyed in the body of the if statement"
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@r@
local idexpression t;
identifier err;
identifier f;
position p;
@@
f(...)
{
<+...
(
err = starpu_task_submit(t);
|
int err = starpu_task_submit(t);
)
if@p(
(
err == -ENODEV
|
err != 0
|
STARPU_UNLIKELY(err == -ENODEV)
|
STARPU_UNLIKELY(err != 0)
)
 )
{
... when != starpu_task_destroy(t);
    when != exit(...);
    when != STARPU_ASSERT(...);
    when != return 77;
}
...+>
}

// Context mode.
@depends on r && context@
position r.p;
@@
*if@p(...) { ... }

// Org mode.
@script:python depends on r && org@
p << r.p;
t << r.t;
f << r.f;
@@
coccilib.org.print_todo(p[0], orgmsg % (f,t))

// Patch mode.
// XXX: Instead of "..." we could use a statement list (statement list SS). But
// it does not seem to work with if there is a "return" statement in the body
// of the if condition.
// Using "..." makes the patch ugly, but this may be fixed in a future version
// of spatch.
@depends on r && patch@
local idexpression r.t;
position r.p;
identifier r.f;
@@
f(...)
{
<+...
if@p (...)
(
{
...
+ t->destroy = 0;
+ starpu_task_destroy(t);
return ...;
}
|
{
...
+ t->destroy = 0;
+ starpu_task_destroy(t);
}
)
...+>
}

// Report mode.
@script:python depends on r && report@
p << r.p;
t << r.t;
f << r.f;
@@
coccilib.report.print_report(p[0], msg % (f,t))
