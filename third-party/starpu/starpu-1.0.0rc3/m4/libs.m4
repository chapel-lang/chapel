# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011  Université de Bordeaux 1
#
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

# STARPU_SEARCH_LIBS(NAME, FUNCTION, SEARCH-LIBS,
#                    [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#                    [OTHER-LIBRARIES])
#
# Like AC_SEARCH_LIBS, but puts -l flags into $1_LDFLAGS instead of LIBS, and
# AC_SUBSTs it
AC_DEFUN([STARPU_SEARCH_LIBS], [dnl
	_LIBS_SAV="$LIBS"
	LIBS=""
	AC_SEARCH_LIBS([$2], [$3], [$4], [$5], [$6])
	STARPU_$1_LDFLAGS="$STARPU_$1_LDFLAGS $LIBS"
	LIBS=$_LIBS_SAV
	AC_SUBST(STARPU_$1_LDFLAGS)
])dnl

# STARPU_CHECK_LIB(NAME, LIBRARY, FUNCTION,
#                  [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#                  [OTHER-LIBRARIES])
#
# Like AC_CHECK_LIB, but puts -l flags into $1_LDFLAGS instead of LIBS, and
# AC_SUBSTs it
AC_DEFUN([STARPU_CHECK_LIB], [dnl
	_LIBS_SAV="$LIBS"
	LIBS=""
	AC_CHECK_LIB([$2], [$3], [$4], [$5], [$6])
	STARPU_$1_LDFLAGS="$STARPU_$1_LDFLAGS $LIBS"
	LIBS=$_LIBS_SAV
	AC_SUBST(STARPU_$1_LDFLAGS)
])dnl

# STARPU_HAVE_LIBRARY(NAME, LIBRARY,
#                     [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#                     [OTHER-LIBRARIES])
# Like AC_HAVE_LIBRARY, but puts -l flags into $1_LDFLAGS instead of LIBS, and
# AC_SUBSTs it
AC_DEFUN([STARPU_HAVE_LIBRARY], [dnl
STARPU_CHECK_LIB([$1], [$2], main, [$3], [$4], [$5])
])dnl
