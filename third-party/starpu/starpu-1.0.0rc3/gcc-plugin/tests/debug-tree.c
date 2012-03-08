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

#pragma starpu debug_tree int			  /* (note "debug_tree") */

int
foo (void)
{
#pragma starpu debug_tree foo			  /* (note "debug_tree")  */

  static int x = 2;
#pragma starpu debug_tree x			  /* (note "debug_tree") */

  return x;
}

#pragma starpu debug_tree void int foo /* (note "debug_tree") *//* (warning "extraneous") */
