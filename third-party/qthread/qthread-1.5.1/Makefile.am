# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# The config files here are included because, if I build the distribution
# tarball with AC 2.60 or better, they aren't included by acinclude.m4; but I
# want them distributed in case someone with AC 2.59 wants to run autoreconf
EXTRA_DIST = \
			 config/ac_prog_cc_c99.m4 \
			 config/ac_use_system_extensions.m4 \
			 shims/chapel/include/tasks-qthreads.h \
			 shims/chapel/src/tasks-qthreads.c \
			 shims/chapel/src/Makefile.include \
			 shims/chapel/src/Makefile \
			 shims/chapel/src/Makefile.share \
			 shims/chapel/README \
			 autogen.sh

SUBDIRS = man include src test

ACLOCAL_AMFLAGS = -I config

DISTCHECK_CONFIGURE_FLAGS = $(EXTRA_DISTCHECK_OPTIONS)

CONFIGURE_DEPENDENCIES = config/version-gen NEWS
