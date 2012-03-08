/* GCC-StarPU
   Copyright (C) 2012 Institut National de Recherche en Informatique et Automatique

   GCC-StarPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   GCC-StarPU is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC-StarPU.  If not, see <http://www.gnu.org/licenses/>.  */

/* (instructions compile (cflags "-fplugin-arg-starpu-verbose")) */

#ifndef STARPU_GCC_PLUGIN
# error barf!
#endif

static void my_task (int x)	      /* (note "implementations for task") */
  __attribute__ ((task));

static void my_task_cpu (int x)
  __attribute__ ((task_implementation ("cpu", my_task)));
extern void my_task_cpu_sse (int x)	       /* (note "my_task_cpu_sse") */
  __attribute__ ((task_implementation ("cpu", my_task)));
extern void my_task_opencl (int x)		/* (note "my_task_opencl") */
  __attribute__ ((task_implementation ("opencl", my_task)));
extern void my_task_cuda (int x)		  /* (note "my_task_cuda") */
  __attribute__ ((task_implementation ("cuda", my_task)));

static void
my_task_cpu (int x)				  /* (note "my_task_cpu") */
{
  /* Nothing.  */
}

int
bar (int x)
{
  my_task (x);					  /* (note "calls task") */
  return 42;
}
