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

include $(top_srcdir)/src/pms/ss/version_commons.am
include $(top_srcdir)/src/common.am

if PERFORMANCE_VERSION
performance_LTLIBRARIES=libnanox-ss.la

nanos_perfo_cxxflags=$(common_includes) $(common_perfo_cxxflags) 

libnanox_ss_la_CXXFLAGS=$(nanos_perfo_cxxflags)
libnanox_ss_la_LDFLAGS=$(version_flags)
libnanox_ss_la_LIBADD=$(top_builddir)/src/pms/openmp/performance/libnanox-omp.la
libnanox_ss_la_SOURCES=$(sources)
libnanox_ss_la_DEPENDENCIES=$(top_builddir)/src/pms/openmp/performance/libnanox-omp.la
endif

