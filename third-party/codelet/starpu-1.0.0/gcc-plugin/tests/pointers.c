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


/* The tasks under test.  */

static void my_pointer_task (const int *x, short *y) __attribute__ ((task));

static void my_pointer_task_cpu (const int *, short *)
  __attribute__ ((task_implementation ("cpu", my_pointer_task)));
static void my_pointer_task_opencl (const int *, short *)
  __attribute__ ((task_implementation ("opencl", my_pointer_task)));

static void
my_pointer_task_cpu (const int *x, short *y)
{
  printf ("%s: x = %p, y = %p\n", __func__, x, y);
}

static void
my_pointer_task_opencl (const int *x, short *y)
{
  printf ("%s: x = %p, y = %p\n", __func__, x, y);
}



static void my_mixed_task (int *x, unsigned char z, const short *y)
  __attribute__ ((task));
static void my_mixed_task_cpu (int *, unsigned char, const short *)
  __attribute__ ((task_implementation ("cpu", my_mixed_task)));
static void my_mixed_task_opencl (int *, unsigned char, const short *)
  __attribute__ ((task_implementation ("opencl", my_mixed_task)));

static void
my_mixed_task_cpu (int *x, unsigned char z, const short *y)
{
  printf ("%s: x = %p, y = %p, z = %i\n", __func__, x, y, (int) z);
}

static void
my_mixed_task_opencl (int *x, unsigned char z, const short *y)
{
  printf ("%s: x = %p, y = %p, z = %i\n", __func__, x, y, (int) z);
}



int
main (int argc, char *argv[])
{
#pragma starpu initialize

  static const unsigned char z = 0x77;
  int x[] = { 42 };
  short *y;

  y = malloc (sizeof *y);
  *y = 77;

  /* Register X and Y (don't use the pragma, to avoid mixing concerns in this
     test.)  */

  starpu_data_handle_t handle;

  expected_register_arguments.pointer = x;
  expected_register_arguments.elements = 1;
  expected_register_arguments.element_size = sizeof x[0];
  starpu_vector_data_register (&handle, 0, (uintptr_t) x, 1, sizeof x[0]);

  expected_register_arguments.pointer = y;
  expected_register_arguments.elements = 1;
  expected_register_arguments.element_size = sizeof *y;
  starpu_vector_data_register (&handle, 0, (uintptr_t) y, 1, sizeof *y);

  struct insert_task_argument expected_pointer_task[] =
    {
      { STARPU_R,  x },
      { STARPU_RW, y },
      { 0, 0, 0 }
    };

  expected_insert_task_arguments = expected_pointer_task;

  /* Invoke the task, which should make sure it gets called with
     EXPECTED.  */
  my_pointer_task (x, y);

  assert (tasks_submitted == 1);


  /* Likewise with `my_mixed_task'.  */

  struct insert_task_argument expected_mixed_task[] =
    {
      { STARPU_RW, x },
      { STARPU_VALUE, &z, sizeof z },
      { STARPU_R,  y },
      { 0, 0, 0 }
    };

  expected_insert_task_arguments = expected_mixed_task;

  my_mixed_task (x, 0x77, y);

  assert (tasks_submitted == 2);

  free (y);

  return EXIT_SUCCESS;
}
