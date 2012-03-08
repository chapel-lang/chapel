# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique
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

# `LIBRARIES' would be more appropriate than `LTLIBRARIES' but it
# requires a name prefixed by `lib'.
gccplugin_LTLIBRARIES = starpu.la

starpu_la_SOURCES = starpu.c c-expr.y

# Use the Yacc-compatibility mode so that Bison doesn't error out upon
# reduce/reduce conflicts.
AM_YFLAGS = -y

AM_CPPFLAGS =						\
  -I$(top_srcdir)/include				\
  -I$(GCC_PLUGIN_INCLUDE_DIR) -Wall -DYYERROR_VERBOSE=1

AM_LDFLAGS = -module
