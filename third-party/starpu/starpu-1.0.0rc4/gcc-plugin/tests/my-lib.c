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

/* Example library of tasks.  */

/* (instructions compile) */

#include <my-lib.h>


/* Task implementations: one is `static', one is global.  The codelet
   wrapper, codelet, and task body should be instantiated in this file.  */

static void my_task_opencl (char, const char *, float *, int)
  __attribute__ ((task_implementation ("opencl", my_task)));

void
my_task_cpu (char a, const char *p, float *q, int b)
{
  int i;

  for (i = 0; i < b; i++)
    *q = *p + a;
}

static void
my_task_opencl (char a, const char *p, float *q, int b)
{
  int i;

  for (i = 0; i < b; i++)
    *q = *p + a;
}
