/* GCC-StarPU
   Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique

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

/* Test error handling for the `task' and `task_implementation' attributes.  */

extern void my_external_task (int foo, char *bar) __attribute__ ((task));

static void my_task (int foo, char *bar) __attribute__ ((task));
static void my_task_cpu (int foo, float *bar)    /* (error "type differs") */
  __attribute__ ((task_implementation ("cpu", my_task)));

static void my_task_opencl (long foo, char *bar) /* (error "type differs") */
  __attribute__ ((task_implementation ("opencl", my_task)));

static void my_task_nowhere (int foo, char *bar) /* (warning "unsupported target") */
  __attribute__ ((task_implementation ("does-not-exist", my_task)));

static void my_task_not_quite (int foo, char *bar) /* (error "lacks the 'task' attribute") */
  __attribute__ ((task_implementation ("cpu", my_task_nowhere)));

static int foo /* (error "only applies to function") */
  __attribute__ ((task_implementation ("cpu", my_task)));

static int bar /* (error "only applies to function") */
  __attribute__ ((task, unused));

static int not_a_task __attribute__ ((unused));

static void my_task_almost (int foo, char *bar)    /* (error "not a function") */
  __attribute__ ((task_implementation ("cpu", not_a_task)));

static void my_task_wrong_task_arg (int foo, char *bar)   /* (error "not a function") */
  __attribute__ ((task_implementation ("cpu", 123)));

static void my_task_wrong_target_arg (int foo, char *bar) /* (error "string constant expected") */
  __attribute__ ((task_implementation (123, my_task)));

static void my_task_with_a_body (int foo, char *bar)
  __attribute__ ((task, unused));

extern int my_task_not_void (int foo) /* (error "return type") */
  __attribute__ ((task));

void my_task_that_invokes_task (int x, char *y)
  __attribute__ ((task));

void my_task_that_invokes_task_cpu (int x, char *y)
  __attribute__ ((task_implementation ("cpu", my_task_that_invokes_task)));

/* XXX: The assumption behind this test is that STARPU_USE_GORDON is not
   defined.  */
void my_task_with_no_usable_implementation (int x) /* (error "none of the implementations") */
  __attribute__ ((task));

static void my_task_with_no_usable_implementation_gordon (int x)
  __attribute__ ((task_implementation ("gordon",
				       my_task_with_no_usable_implementation)));

/* XXX: In practice this test fails for large values of `STARPU_NMAXBUFS'.  */
void my_task_with_too_many_pointer_params (/* (error "maximum .* exceeded") */
					   char *x1, char *x2, char *x3,
					   char *x4, char *x5, char *x6,
					   char *x7, char *x8, char *x9,
					   char *xa, char *xb, char *xc,
					   char *xd, char *xe, char *xf,
					   char *xg, char *xh, char *xi)
  __attribute__ ((task));



static void
my_task_cpu (int foo, float *bar)
{
}

static void
my_task_opencl (long foo, char *bar)
{
}

static void
my_task_nowhere (int foo, char *bar)
{
}

static void
my_task_not_quite (int foo, char *bar)
{
}

static void
my_task_almost (int foo, char *bar)
{
}

static void
my_task_wrong_task_arg (int foo, char *bar)
{
}

static void
my_task_wrong_target_arg (int foo, char *bar)
{
}

static void
my_task_with_a_body (int foo, char *bar)  /* (error "must not have a body") */
{
}

void
my_task_that_invokes_task_cpu (int x, char *y)
{
  my_external_task (x, y); /* (error "cannot be invoked from task implementation") */
}

static void
my_task_with_no_usable_implementation_gordon (int x)
{
}
