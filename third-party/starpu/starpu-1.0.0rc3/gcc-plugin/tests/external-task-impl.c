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

/* (instructions compile) */

void the_task (int foo, float bar[foo])
  __attribute__ ((task));

static void the_task_cpu (int foo, float bar[foo])
  __attribute__ ((task_implementation ("cpu", the_task)));

/* Make sure GCC doesn't barf on this one.  */
extern void the_task_cuda (int foo, float bar[foo])
  __attribute__ ((task_implementation ("cuda", the_task)));

static void
the_task_cpu (int foo, float bar[foo])
{
  /* Nothingness.  */
}
