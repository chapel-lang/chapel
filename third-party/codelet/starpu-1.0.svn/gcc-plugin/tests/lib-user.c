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

/* Test whether tasks defined in another compilation unit can actually be
   used.  */

/* (instructions run (dependencies "my-lib.c")) */

#include <mocks.h>

#include <my-lib.h>

int
main (int argc, char *argv[])
{
#pragma starpu initialize

  /* Align X so that the assumptions behind `dummy_pointer_to_handle'
     hold.  */
  static const signed char x[] __attribute__ ((aligned (8))) =
    { 0, 1, 2, 3, 4, 5 };

  float y[sizeof x];

  static const char forty_two = 42;
  static const int  sizeof_x = sizeof x;

  struct insert_task_argument expected_pointer_task[] =
    {
      { STARPU_VALUE, &forty_two, sizeof forty_two },
      { STARPU_R,  x },
      { STARPU_RW, y },
      { STARPU_VALUE, &sizeof_x, sizeof sizeof_x },
      { 0, 0, 0 }
    };

  expected_insert_task_arguments = expected_pointer_task;

  expected_register_arguments.pointer = (void *) x;
  expected_register_arguments.elements = sizeof x / sizeof x[0];
  expected_register_arguments.element_size = sizeof x[0];
#pragma starpu register x

  expected_register_arguments.pointer = y;
  expected_register_arguments.elements = sizeof y / sizeof y[0];
  expected_register_arguments.element_size = sizeof y[0];
#pragma starpu register y

  /* Invoke the task, which should make sure it gets called with
     EXPECTED.  */
  my_task (42, x, y, sizeof x);

  assert (tasks_submitted == 1);

#pragma starpu shutdown

  return EXIT_SUCCESS;
}
