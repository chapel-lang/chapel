SUBDIRS = partSendPrototype

DIST_SUBDIRS = $(SUBDIRS)

.PHONY: buildall buildextra benchmarks

finepoints_benchmarks = partSendPrototype/partTest

benchmarks: buildextra
buildall: buildextra

buildextra:
	for DIR in $(SUBDIRS) ; do \
		$(MAKE) -C $$DIR buildall ; \
	done

partSendPrototype/partTest:
	$(MAKE) -C partSendPrototype buildall

AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la
