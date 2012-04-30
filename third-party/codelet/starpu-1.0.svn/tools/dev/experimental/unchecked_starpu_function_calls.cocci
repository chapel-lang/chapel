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
 * The return values of functions such as starpu_init(), starpu_task_submit(),
 * starpu_task_wait() should _always_ be checked.
 */

virtual context
virtual org
virtual patch
virtual report

@initialize:python depends on report || org@
msg = "Unchecked call to %s"
from re import sub
orgmsg = sub(r'(%[a-z])', r'=\1=', msg)

@unchecked_starpu_func_call@
identifier f;
position p;
identifier starpu_function =~ "\bstarpu_(init|task_(submit|wait))\b";
@@
f(...)
{
<+...
starpu_function@p(...);
...+>
}


// Context mode.
@depends on unchecked_starpu_func_call && context@
position unchecked_starpu_func_call.p;
identifier unchecked_starpu_func_call.starpu_function;
identifier unchecked_starpu_func_call.f;
@@
f(...)
{
<+...
* starpu_function@p(...);
...+>
}

// Org mode.
@script:python depends on unchecked_starpu_func_call && org@
f << unchecked_starpu_func_call.starpu_function;
p << unchecked_starpu_func_call.p;
@@
coccilib.org.print_todo(p[0], orgmsg % f)

// Patch mode.
@has_ret depends on unchecked_starpu_func_call@
identifier unchecked_starpu_func_call.f;
identifier ret;
identifier starpu_func =~ "^starpu_";
@@
f(...)
{
...
int ret;
...
ret = starpu_func(...);
...
}

@script:python stringify depends on patch@
function_name << unchecked_starpu_func_call.starpu_function;
starpu_function_name;
@@
coccinelle.starpu_function_name = '"'+str(function_name)+'"'


// In this rule, "ret" is probably a good choice. This should be checked when
// reviewing the patches, though.
@depends on unchecked_starpu_func_call && has_ret && patch@
identifier unchecked_starpu_func_call.f;
identifier has_ret.ret;
identifier unchecked_starpu_func_call.starpu_function;
identifier stringify.starpu_function_name;
@@
f(...)
{
<...
- starpu_function(
+ ret = starpu_function(
...);
+ STARPU_CHECK_RETURN_VALUE(ret, starpu_function_name);
...>
}

// In this rule, we use a variable called "ret" that is probably not declared.
// Obviously, the patches should be read before being committed !
@depends on unchecked_starpu_func_call && !has_ret && patch@
identifier unchecked_starpu_func_call.f;
identifier unchecked_starpu_func_call.starpu_function;
identifier stringify.starpu_function_name;
@@
f(...)
{
<...
- starpu_function(
+ ret = starpu_function(
...);
+ STARPU_CHECK_RETURN_VALUE(ret, starpu_function_name);
...>
}

// Report mode.
@script:python depends on unchecked_starpu_func_call && report@
f << unchecked_starpu_func_call.starpu_function;
p << unchecked_starpu_func_call.p;
@@
coccilib.report.print_report(p[0], msg % f)
