#####################################################################################
#      Copyright 2009 Barcelona Supercomputing Center                               #
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

include $(top_srcdir)/src/core/version_commons.am
include $(top_srcdir)/src/common.am

if DEBUG_VERSION
debug_LTLIBRARIES=libnanox.la

nanos_cxxflags=$(common_includes) $(common_debug_cxxflags) 
nanos_ldflags=$(version_flags) @NANOS_CONFIG_LIBDIRS@ -Wl,-rpath,$(debugdir)

libnanox_la_CFLAGS=$(common_cflags) $(common_debug_cflags)
libnanox_la_CXXFLAGS=$(nanos_cxxflags)
libnanox_la_LDFLAGS=$(nanos_ldflags)
libnanox_la_LIBADD=$(common_libadd) @NANOS_CONFIG_LIBS_DEBUG@
libnanox_la_SOURCES=$(common_sources)
libnanox_la_DEPENDENCIES=$(common_libadd) @NANOS_CONFIG_LIBS_DEBUG@
endif

