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

#undef NDEBUG

#include <mocks.h>


/* The task under test.  */

static void my_scalar_task (int x, unsigned char y, int z) __attribute__ ((task));

static void my_scalar_task_cpu (int, unsigned char, int)
  __attribute__ ((task_implementation ("cpu", my_scalar_task)));
static void my_scalar_task_opencl (int, unsigned char, int)
  __attribute__ ((task_implementation ("opencl", my_scalar_task)));

static void
my_scalar_task_cpu (int x, unsigned char y, int z)
{
  printf ("%s: x = %i, y = %i, z = %i\n", __func__, x, (int) y, z);
}

static void
my_scalar_task_opencl (int x, unsigned char y, int z)
{
  printf ("%s: x = %i, y = %i, z = %i\n", __func__, x, (int) y, z);
}


/* Another task, where task implementation declarations are interleaved with
   task definitions.  */

static void my_other_task (int x) __attribute__ ((task));

static void my_other_task_cpu (int)
  __attribute__ ((task_implementation ("cpu", my_other_task)));
static void my_other_task_cpu_bis (int)
  __attribute__ ((task_implementation ("cpu", my_other_task)));

static void
my_other_task_cpu (int x)
{
  printf ("cpu\n");
}

static void
my_other_task_cpu_bis (int x)
{
  printf ("second cpu implementation\n");
}

static void my_other_task_opencl (int)
  __attribute__ ((task_implementation ("opencl", my_other_task)));

static void
my_other_task_opencl (int x)
{
  printf ("opencl\n");
}




int
main (int argc, char *argv[])
{
#pragma starpu initialize
  assert (initialized == 1);

#pragma starpu hello

  int x = 42, z = 99;
  unsigned char y = 77;
  long y_as_long_int = 77;

  struct insert_task_argument expected[] =
    {
      { STARPU_VALUE, &x, sizeof x },
      { STARPU_VALUE, &y, sizeof y },
      { STARPU_VALUE, &z, sizeof z },
      { 0, 0, 0 }
    };

  expected_insert_task_arguments = expected;

  /* Invoke the task, which should make sure it gets called with
     EXPECTED.  */
  my_scalar_task (x, y, z);

  /* Invoke the task using literal constants instead of variables.  */
  my_scalar_task (42, y, z);
  my_scalar_task (x, 77, z);
  my_scalar_task (x, y, 99);

  my_scalar_task (42, 77, z);
  my_scalar_task (x, 77, 99);
  my_scalar_task (42, y, 99);

  my_scalar_task (42, 77, 99);

  my_scalar_task (42, y_as_long_int, 99);

  struct insert_task_argument expected2[] =
    {
      { STARPU_VALUE, &x, sizeof x },
      { 0, 0, 0 }
    };

  expected_insert_task_arguments = expected2;

  my_other_task (42);

  assert (tasks_submitted == 10);

#pragma starpu shutdown
  assert (initialized == 0);

  return EXIT_SUCCESS;
}
