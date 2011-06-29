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

include $(top_srcdir)/src/plugins/sched/version_commons.am
include $(top_srcdir)/src/common.am

if DEBUG_VERSION
debug_LTLIBRARIES = \
        libnanox-sched-default.la\
	libnanox-sched-bf.la \
	libnanox-sched-dbf.la \
 	libnanox-sched-cilk.la \
 	libnanox-sched-wf.la \
 	libnanox-sched-affinity.la \
	$(END)

nanos_debug_cxxflags=$(common_includes) $(common_debug_cxxflags)

libnanox_sched_default_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_default_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_default_la_SOURCES = $(default_sources)

libnanox_sched_bf_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_bf_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_bf_la_SOURCES = $(bf_sources)

libnanox_sched_dbf_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_dbf_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_dbf_la_SOURCES = $(dbf_sources)

libnanox_sched_wf_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_wf_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_wf_la_SOURCES = $(wf_sources)
 
libnanox_sched_cilk_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_cilk_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_cilk_la_SOURCES = $(cilk_sources)
 
libnanox_sched_affinity_la_CXXFLAGS = $(nanos_debug_cxxflags)
libnanox_sched_affinity_la_LDFLAGS = $(ld_plugin_flags)
libnanox_sched_affinity_la_SOURCES = $(affinity_sources)
endif

