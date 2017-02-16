SUBDIRS = hpccg

DIST_SUBDIRS = $(SUBDIRS) hpccg_rose

.PHONY: buildall buildextra benchmarks

mantevo_benchmarks = hpccg/hpccg

if COMPILE_ROSE_EXTENSIONS
SUBDIRS += hpccg_rose
mantevo_benchmarks += hpccg_rose/hpccg
endif

benchmarks: buildextra
buildall: buildextra

buildextra:
	for DIR in $(SUBDIRS) ; do \
		$(MAKE) -C $$DIR buildall ; \
	done

hpccg/hpccg:
	$(MAKE) -C hpccg buildall

hpccg_rose/hpccg:
	$(MAKE) -C hpccg_rose buildall

AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la
