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

include $(top_srcdir)/src/plugins/throttle/version_commons.am
include $(top_srcdir)/src/common.am

if PERFORMANCE_VERSION
performance_LTLIBRARIES = \
	libnanox-throttle-dummy.la \
	libnanox-throttle-numtasks.la \
	libnanox-throttle-idlethreads.la \
	libnanox-throttle-taskdepth.la \
	libnanox-throttle-readytasks.la \
	$(END)

nanos_perfo_cxxflags=$(common_includes) $(common_perfo_cxxflags)

libnanox_throttle_dummy_la_CXXFLAGS = $(nanos_perfo_cxxflags)
libnanox_throttle_dummy_la_LDFLAGS = $(ld_plugin_flags)
libnanox_throttle_dummy_la_SOURCES = $(dummy_sources)

libnanox_throttle_numtasks_la_CXXFLAGS = $(nanos_perfo_cxxflags)
libnanox_throttle_numtasks_la_LDFLAGS = $(ld_plugin_flags)
libnanox_throttle_numtasks_la_SOURCES = $(numtasks_sources)

libnanox_throttle_idlethreads_la_CXXFLAGS = $(nanos_perfo_cxxflags)
libnanox_throttle_idlethreads_la_LDFLAGS = $(ld_plugin_flags)
libnanox_throttle_idlethreads_la_SOURCES = $(idlethreads_sources)

libnanox_throttle_taskdepth_la_CXXFLAGS = $(nanos_perfo_cxxflags)
libnanox_throttle_taskdepth_la_LDFLAGS = $(ld_plugin_flags)
libnanox_throttle_taskdepth_la_SOURCES = $(taskdepth_sources)

libnanox_throttle_readytasks_la_CXXFLAGS = $(nanos_perfo_cxxflags)
libnanox_throttle_readytasks_la_LDFLAGS = $(ld_plugin_flags)
libnanox_throttle_readytasks_la_SOURCES = $(readytasks_sources)
endif

