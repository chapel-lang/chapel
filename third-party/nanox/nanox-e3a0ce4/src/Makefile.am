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

SUBDIRS = arch support plugins core apis pms utils

EXTRA_DIST = plugins-list.in

noinst_DATA = plugins-list

CLEANFILES = plugins-list

if PERFORMANCE_VERSION
perfo=performance
endif
if DEBUG_VERSION
debug=debug
endif
if INSTRUMENTATION_VERSION
instr=instrumentation
endif
if INSTRUMENTATION_DEBUG_VERSION
instrdebug=instrumentation_debug
endif

plugins-list: $(perfo) $(debug) $(instr) $(instrdebug)
	rm -f plugins-list
	if test -e plugins-list-perfo; then \
	cat plugins-list-perfo >> plugins-list; \
	fi
	if test -e plugins-list-debug; then \
	cat plugins-list-debug >> plugins-list; \
	fi
	if test -e plugins-list-instr; then \
	cat plugins-list-instr >> plugins-list; \
	fi
	if test -e plugins-list-instrdebug; then \
	cat plugins-list-instrdebug >> plugins-list; \
	fi
	rm -rf plugins-list-perfo plugins-list-debug plugins-list-instr plugins-list-instrdebug

performance: plugins-list.in
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_srcdir=$(srcdir) ; \
	sed \
		-e's|\(.*\)|'"$$tmp_builddir"'/src/\1/performance/.libs|g' \
		$$tmp_srcdir/plugins-list.in > plugins-list-perfo.tmp
	sed \
		-e :a -e N -e's/\n/:/' -e ta \
		plugins-list-perfo.tmp > plugins-list-perfo.tmp2
	sed \
		-e's|\(.*\)|performance=\1|g' \
		plugins-list-perfo.tmp2 > plugins-list-perfo
	rm -f plugins-list-perfo.tmp plugins-list-perfo.tmp2

debug: plugins-list.in
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_srcdir=$(srcdir) ; \
	sed \
		-e's|\(.*\)|'"$$tmp_builddir"'/src/\1/debug/.libs|g' \
		$$tmp_srcdir/plugins-list.in > plugins-list-debug.tmp
	sed \
		-e :a -e N -e's/\n/:/' -e ta \
		plugins-list-debug.tmp > plugins-list-debug.tmp2
	sed \
		-e's|\(.*\)|debug=\1|g' \
		plugins-list-debug.tmp2 > plugins-list-debug
	rm -f plugins-list-debug.tmp plugins-list-debug.tmp2

instrumentation: plugins-list.in
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_srcdir=$(srcdir) ; \
	sed \
		-e's|\(.*\)|'"$$tmp_builddir"'/src/\1/instrumentation/.libs|g' \
		$$tmp_srcdir/plugins-list.in > plugins-list-instr.tmp
	sed \
		-e :a -e N -e's/\n/:/' -e ta \
		plugins-list-instr.tmp > plugins-list-instr.tmp2
	sed \
		-e's|\(.*\)|instrumentation=\1|g' \
		plugins-list-instr.tmp2 > plugins-list-instr
	rm -f plugins-list-instr.tmp plugins-list-instr.tmp2

instrumentation_debug: plugins-list.in
	tmp_builddir=$(abs_top_builddir) ; \
	tmp_srcdir=$(srcdir) ; \
	sed \
		-e's|\(.*\)|'"$$tmp_builddir"'/src/\1/instrumentation-debug/.libs|g' \
		$$tmp_srcdir/plugins-list.in > plugins-list-instrdebug.tmp
	sed \
		-e :a -e N -e's/\n/:/' -e ta \
		plugins-list-instrdebug.tmp > plugins-list-instrdebug.tmp2
	sed \
		-e's|\(.*\)|instrumentation_debug=\1|g' \
		plugins-list-instrdebug.tmp2 > plugins-list-instrdebug
	rm -f plugins-list-instrdebug.tmp plugins-list-instrdebug.tmp2

