# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

SUBDIRS =

AM_CFLAGS = $(HWLOC_CFLAGS) $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)
LIBS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la @LIBS@
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/tools/ -I$(top_srcdir)/mpi/ -I$(top_builddir)/src -I$(top_srcdir)/src

bin_PROGRAMS =
dist_bin_SCRIPTS =

dist_pkgdata_DATA = gdbinit

EXTRA_DIST =					\
  dev/rename.sed				\
  dev/rename.sh

CLEANFILES = *.gcno *.gcda *.linkinfo

bin_PROGRAMS += starpu_calibrate_bus

if STARPU_USE_FXT
bin_PROGRAMS += starpu_fxt_tool starpu_fxt_stats

starpu_fxt_tool_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS) $(FXT_CFLAGS)
starpu_fxt_tool_LDADD = $(FXT_LIBS)

starpu_fxt_stats_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS) $(FXT_CFLAGS)
starpu_fxt_stats_LDADD = $(FXT_LIBS)
endif

bin_PROGRAMS += \
	starpu_perfmodel_display	\
	starpu_perfmodel_plot 		\
	starpu_machine_display

noinst_PROGRAMS =	cbc2paje lp2paje

dist_bin_SCRIPTS +=	\
	starpu_workers_activity


if STARPU_HAVE_HELP2MAN
starpu_calibrate_bus.1: starpu_calibrate_bus$(EXEEXT)
	help2man --no-discard-stderr -N --output=$@ ./starpu_calibrate_bus$(EXEEXT)
starpu_machine_display.1: starpu_machine_display$(EXEEXT)
	help2man --no-discard-stderr -N --output=$@ ./starpu_machine_display$(EXEEXT)
starpu_perfmodel_display.1: starpu_perfmodel_display$(EXEEXT)
	help2man --no-discard-stderr -N --output=$@ ./starpu_perfmodel_display$(EXEEXT)
starpu_perfmodel_plot.1: starpu_perfmodel_plot$(EXEEXT)
	help2man --no-discard-stderr -N --output=$@ ./starpu_perfmodel_plot$(EXEEXT)
starpu_workers_activity.1: starpu_workers_activity$(EXEEXT)
	chmod +x starpu_workers_activity$(EXEEXT)
	help2man --no-discard-stderr -N --output=$@ ./starpu_workers_activity$(EXEEXT)

clean-local:
	$(RM) starpu_calibrate_bus.1 starpu_machine_display.1 starpu_perfmodel_display.1 starpu_perfmodel_plot.1 starpu_workers_activity.1

dist_man1_MANS =\
	starpu_calibrate_bus.1 \
	starpu_machine_display.1 \
	starpu_perfmodel_display.1 \
	starpu_perfmodel_plot.1	\
	starpu_workers_activity.1
endif

showcheck:
	-cat /dev/null
