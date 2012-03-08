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

#ifndef MY_LIB_H
#define MY_LIB_H

extern void my_task (char, const char *, float *, int)
  __attribute__ ((task));

/* One of the implementations of MY_TASK.  Since it's `extern', this should
   not trigger generation of the codelet, wrapper, etc.  */
extern void my_task_cpu (char, const char *, float *, int)
  __attribute__ ((task_implementation ("cpu", my_task)));

#endif /* MY_LIB_H */
