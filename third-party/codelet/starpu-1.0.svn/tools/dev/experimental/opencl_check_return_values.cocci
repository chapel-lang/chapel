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

@ignored_return_value@
identifier opencl_func =~ "^cl[A-Z]";
position p;
@@
opencl_func@p(...);

@depends on ignored_return_value && patch@
identifier ignored_return_value.opencl_func;
position ignored_return_value.p;
@@
- opencl_func@p(
+ err = opencl_func(
...);
+ if (STARPU_UNLIKELY(err == clSuccess))
+ STARPU_OPENCL_REPORT_ERROR(err);

@depends on ignored_return_value && context@
identifier ignored_return_value.opencl_func;
@@
* opencl_func(...);

@script:python depends on ignored_return_value && report@
p << ignored_return_value.p;
func << ignored_return_value.opencl_func;
@@
msg = "Ignoring the return value of %s." % func
coccilib.report.print_report(p[0], msg)

@script:python depends on ignored_return_value && org@
p << ignored_return_value.p;
func << ignored_return_value.opencl_func;
@@
msg = "Ignoring the return value of =%s=." % func
coccilib.org.print_todo(p[0], msg)
