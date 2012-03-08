/* GCC-StarPU
   Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique

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

/* (instructions run (ldflags "-lstarpu-1.0")) */

#undef NDEBUG

#include <stdlib.h>
#include <assert.h>
#include <string.h>


/* The task under test.  */

static void my_pointer_task (const int *x, char a, long long *y, int b)
  __attribute__ ((task));

static void my_pointer_task_cpu (const int *x, char a, long long *y, int b)
  __attribute__ ((task_implementation ("cpu", my_pointer_task), noinline));

static int implementations_called;

/* The input arguments.  */
static const int pointer_arg1[] = { 42, 1, 2, 3, 4, 5 };
static long long *pointer_arg2;

static void
my_pointer_task_cpu (const int *x, char a, long long *y, int b)
{
  implementations_called |= STARPU_CPU;
  assert (x == pointer_arg1);
  assert (y == pointer_arg2);
  assert (a == 'S');
  assert (b == 42);
}



int
main (int argc, char *argv[])
{
#define COUNT 100
  pointer_arg2 = malloc (COUNT * sizeof *pointer_arg2);
  memset (pointer_arg2, 0x77, COUNT * sizeof *pointer_arg2);

#pragma starpu initialize

  /* Register POINTER_ARG1 and POINTER_ARG2.  */
#pragma starpu register pointer_arg1
#pragma starpu register pointer_arg2 COUNT

  /* Invoke the task, which should make sure it gets called with
     EXPECTED.  */
  my_pointer_task (pointer_arg1, 'S', pointer_arg2, 42);
#pragma starpu wait
  assert (implementations_called == STARPU_CPU);

  implementations_called = 0;

  /* Same, but with implicit integer type conversion.  */
  my_pointer_task (pointer_arg1, (long long) 'S', pointer_arg2, (char) 42);
#pragma starpu wait
  assert (implementations_called == STARPU_CPU);

  starpu_shutdown ();

  free (pointer_arg2);

  return EXIT_SUCCESS;
#undef COUNT
}
