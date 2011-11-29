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

include $(top_srcdir)/src/pms/chapel/version_commons.am
include $(top_srcdir)/src/common.am

if CHAPEL_SUPPORT
if INSTRUMENTATION_VERSION
instrumentation_LTLIBRARIES=libnanox-chpl.la

nanos_instr_cxxflags=$(common_includes) $(common_instr_cxxflags) 

libnanox_chpl_la_CXXFLAGS=$(nanos_instr_cxxflags) $(chpl_flags)
libnanox_chpl_la_LDFLAGS=$(version_flags)
libnanox_chpl_la_LIBADD=$(top_builddir)/src/core/instrumentation/libnanox.la
libnanox_chpl_la_SOURCES=$(sources)
libnanox_chpl_la_DEPENDENCIES=$(top_builddir)/src/core/instrumentation/libnanox.la
endif
endif

