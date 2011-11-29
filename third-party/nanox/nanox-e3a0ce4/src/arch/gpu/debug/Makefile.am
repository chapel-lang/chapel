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

include $(top_srcdir)/src/arch/gpu/version_commons.am
include $(top_srcdir)/src/common.am

if DEBUG_VERSION
noinst_LTLIBRARIES = libgpu.la
debug_LTLIBRARIES = libnanox-pe-gpu.la libnanox-gpu-cublas.la

nanos_debug_cxxflags=$(common_includes) $(common_debug_cxxflags) $(gpu_includes)

libgpu_la_CXXFLAGS=$(nanos_debug_cxxflags)
libgpu_la_LDFLAGS=$(gpu_ldflags)
libgpu_la_SOURCES=$(gpu_sources)

libnanox_pe_gpu_la_CXXFLAGS=$(nanos_debug_cxxflags)
libnanox_pe_gpu_la_LDFLAGS=$(ld_plugin_flags) $(gpu_ldflags)
libnanox_pe_gpu_la_SOURCES=$(pe_gpu_sources)

libnanox_gpu_cublas_la_CXXFLAGS=$(nanos_debug_cxxflags)
libnanox_gpu_cublas_la_LDFLAGS=$(ld_plugin_flags) $(gpu_cublas_ldflags)
libnanox_gpu_cublas_la_SOURCES=$(gpu_cublas_sources)
endif

