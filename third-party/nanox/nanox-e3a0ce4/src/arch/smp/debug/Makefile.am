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

include $(top_srcdir)/src/arch/smp/version_commons.am
include $(top_srcdir)/src/common.am

if DEBUG_VERSION
noinst_LTLIBRARIES = libsmp.la
debug_LTLIBRARIES = libnanox-pe-smp.la
debug_LTLIBRARIES+= libnanox-pe-smp-numa.la

if SMP_SUPPORTS_ULT
smp_ult_sources=$(ult_sources)
smp_ult_flags=$(ult_flags)
endif

nanos_debug_cxxflags=$(common_includes) $(common_debug_cxxflags) $(smp_ult_flags)

libsmp_la_CXXFLAGS=$(nanos_debug_cxxflags)
libsmp_la_SOURCES=$(smp_sources) $(smp_ult_sources)

libnanox_pe_smp_la_CXXFLAGS=$(nanos_debug_cxxflags)
libnanox_pe_smp_la_LDFLAGS=$(ld_plugin_flags)
libnanox_pe_smp_la_SOURCES=$(pe_smp_sources)

libnanox_pe_smp_numa_la_CXXFLAGS=$(nanos_debug_cxxflags) $(smp_numa_flags)
libnanox_pe_smp_numa_la_LDFLAGS=$(ld_plugin_flags)
libnanox_pe_smp_numa_la_SOURCES=$(pe_smp_sources)
endif

