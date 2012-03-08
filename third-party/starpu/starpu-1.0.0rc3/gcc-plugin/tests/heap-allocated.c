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

static void
foo (size_t size)
{
  /* See ISO C99, Section 6.7.5.2 ("Array Declarators") and Section 6.7.6
     ("Type names").  */

  float *test_array_parm (float m[size][23], int x, int y)
  {
    assert ((char *) &m[0][0] != (char *) &m);
    return &m[x][y];
  }

  size_t minus_one = size - 1;
  float *test_array_static_parm (float m[static minus_one][23], int x, int y)
  {
    assert ((char *) &m[0][0] != (char *) &m);
    return &m[x][y];
  }

  float *test_pointer_parm (float *m, int x, int y)
  {
    return &m[23 * x + y];
  }

  expected_malloc_argument = size * 23 * sizeof (float);

  /* The idea is that this code should be compilable both with and without
     the attribute.  */
  float m[size][23] __attribute__ ((heap_allocated));

  assert (malloc_calls == 1);

  /* `&m' points to the on-stack area that stores the underlying pointer,
     whereas `m' and `m[0][0]' should point to the heap-allocated area.  */
  assert ((char *) &m != (char *) m);
  assert ((char *) &m[0][0] != (char *) &m);
  assert ((char *) &m[0][0] == (char *) m);

  /* Make sure "array arithmetic" works.  */
  assert ((char *) &m[0][1] - (char *) &m[0][0] == sizeof m[0][0]);
  assert ((char *) &m[1][0] - (char *) &m[0][22] == sizeof m[0][0]);

  unsigned int x, y;
  for (x = 0; x < size; x++)
    for (y = 0; y < 23; y++)
      {
	assert (&m[x][y] == test_array_parm (m, x, y));
	assert (&m[x][y] == test_array_static_parm (m, x, y));
	assert (&m[x][y] == test_pointer_parm ((float *) m, x, y));
      }

  /* Freed when going out of scope.  */
  expected_free_argument = m;
}

int
main (int argc, char *argv[])
{
#pragma starpu initialize

  /* Choose the size such that the process would most likely segfault if
     `foo' tried to allocate this much data on the stack.  */
  foo (100000);

  assert (free_calls == 1);

  return EXIT_SUCCESS;
}
