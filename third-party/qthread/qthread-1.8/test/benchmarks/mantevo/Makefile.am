SUBDIRS = hpccg

.PHONY: buildall buildextra benchmarks

benchmarks: buildextra
buildall: buildextra

mantevo_benchmarks = hpccg

buildextra:
	$(MAKE) -C hpccg hpccg

INCLUDES = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la
