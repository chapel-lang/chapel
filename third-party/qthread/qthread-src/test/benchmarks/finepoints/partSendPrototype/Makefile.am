.PHONY: buildall buildextra benchmarks

benchmarks: partTest
buildall: partTest
buildextra: partTest

EXTRA_PROGRAMS = partTest
CLEANFILES = partTest

CC=mpicc #$(MPICC)
CXX=mpic++ #$(MPICXX)

AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

partTest_SOURCES = mpiBase.cc \
	mpiPart.cc \
	mpiBaseTest1.cc \
	mpiBaseTest11.cc \
	mpiBaseTest2.cc \
	mpiBaseTest21.cc \
	mpiBaseTest3.cc \
	mpiBaseTest31.cc \
	mpiBaseTest31t.cc \
	mpiBaseTest32.cc \
	mpiBaseTest5.cc

partTest_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS) -DUSING_QTHREADS -DWALL

