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

#define __output __attribute__ ((output))

/* XXX: Currently, since `output' is a type attribute, we have no way to
   restrict its use to PARM_DECLs of tasks.  */

void f (__output int x)				  /* (error "not allowed") */
  __attribute__ ((task));

__output void g (int x);			  /* (error "not allowed") */
