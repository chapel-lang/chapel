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

/* Test whether `#pragma starpu release ...' generates the right code.  */

#undef NDEBUG

#include <mocks.h>

static void
foo (char *x, int foo)
{
  expected_release_arguments.pointer = x;
#pragma starpu release x
}


int
main (int argc, char *argv[])
{
#pragma starpu initialize

  int x[123];
  static char z[345];

  expected_register_arguments.pointer = x;
  expected_register_arguments.elements = 123;
  expected_register_arguments.element_size = sizeof x[0];
#pragma starpu register x

  expected_release_arguments.pointer = x;
#pragma starpu release x

  expected_register_arguments.pointer = z;
  expected_register_arguments.elements = sizeof z;
  expected_register_arguments.element_size = sizeof z[0];
#pragma starpu register z

  expected_release_arguments.pointer = z;
#pragma starpu release z

  foo (z, 345);

  assert (data_register_calls == 2);
  assert (data_release_calls == 3);

  return EXIT_SUCCESS;
}
