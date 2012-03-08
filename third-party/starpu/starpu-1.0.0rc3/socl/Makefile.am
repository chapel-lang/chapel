# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique
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

SUBDIRS = src examples

EXTRA_DIST = README

libsocl_la_includedir=$(includedir)/starpu/$(STARPU_EFFECTIVE_VERSION)/socl/CL

libsocl_la_include_HEADERS = \
  include/CL/cl.h \
  include/CL/cl_ext.h \
  include/CL/cl_gl.h \
  include/CL/cl_gl_ext.h \
  include/CL/cl_platform.h \
  include/CL/cl_starpu.h \
  include/CL/opencl.h \
  include/CL/cl.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = socl-1.0.pc

showcheck:
	for i in $(SUBDIRS) ; do \
		make -C $$i showcheck ; \
	done
