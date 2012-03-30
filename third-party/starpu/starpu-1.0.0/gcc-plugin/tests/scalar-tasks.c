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

/* (instructions run (ldflags "-lstarpu-1.0")) */

#undef NDEBUG

#include <stdlib.h>
#include <assert.h>


/* The task under test.  */

static void my_scalar_task (int x, int y) __attribute__ ((task));

static void my_scalar_task_opencl (int, int)
  __attribute__ ((task_implementation ("opencl", my_scalar_task), noinline));

static int implementations_called;

/* CPU implementation of `my_scalar_task'.  */
static void
my_scalar_task (int x, int y)
{
  implementations_called |= STARPU_CPU;
  assert (x == 42);
  assert (y == 77);
}

static void
my_scalar_task_opencl (int x, int y)
{
  implementations_called |= STARPU_OPENCL;
  assert (x == 42);
  assert (y == 77);
}


int
main (int argc, char *argv[])
{
#pragma starpu initialize

  /* Invoke the task, which should make sure it gets called with
     EXPECTED.  */
  my_scalar_task (42, 77);

#pragma starpu wait

  assert ((implementations_called & STARPU_CPU)
	  || (implementations_called & STARPU_OPENCL));

  assert ((implementations_called & ~(STARPU_CPU | STARPU_OPENCL)) == 0);

  starpu_shutdown ();

  return EXIT_SUCCESS;
}
