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

SUBDIRS = performance debug instrumentation instrumentation-debug

include $(top_srcdir)/src/common.am

EXTRA_DIST= \
	x86/stack.cpp \
	x86/stack.s \
	x86-64/stack.cpp \
	x86-64/stack.s \
	ia64/stack.cpp \
	ia64/stack.s \
	ppc32/stack.cpp \
	ppc32/stack.s \
	ppc64/stack.cpp \
	ppc64/stack.s \
        smp_ult.hpp \
	$(END)

