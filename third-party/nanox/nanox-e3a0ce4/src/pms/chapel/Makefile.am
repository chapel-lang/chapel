#####################################################################################
#      Copyright 2010 Barcelona Supercomputing Center                               #
#                                                                                   #
#      This file is part of the NANOS++ library.                                    #
#                                                                                   #
#      NANOS++ is free software: you can redistribute it and/or modify              #
#      it under the terms of the GNU Lesser General Public License as published by  #
#      the Free Software Foundation, either version 3 of the License, or            #
#      (at your option) any later version.                                          #
#                                                                                   #
#      NANOS++ is distributed in the hope that it will be useful,                   #
#      but WITHOUT ANY WARRANTY; without even the implied warranty of               #
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
#      GNU Lesser General Public License for more details.                          #
#                                                                                   #
#      You should have received a copy of the GNU Lesser General Public License     #
#      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             #
#####################################################################################

SUBDIRS = performance debug instrumentation instrumentation-debug

include $(top_srcdir)/src/common.am
include version_commons.am

if CHAPEL_SUPPORT

pkginclude_HEADERS=\
        chpl_nanos.h\
	$(END)

lib_LIBRARIES=libnanox-chpl-init.a

libnanox_chpl_init_a_SOURCES= $(top_srcdir)/src/pms/chapel/chpl_init.c
libnanox_chpl_init_a_CFLAGS= $(common_includes) $(chpl_flags)

endif


