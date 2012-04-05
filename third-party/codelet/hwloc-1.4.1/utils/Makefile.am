# Copyright © 2009-2010 inria.  All rights reserved.
# Copyright © 2009-2011 Université Bordeaux 1
# Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
#
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

LDADD = $(HWLOC_top_builddir)/src/libhwloc.la

EXTRA_DIST = test-hwloc-assembler.input1 test-hwloc-assembler.input2 test-hwloc-assembler.output test-hwloc-calc.output test-hwloc-distances.output test-hwloc-distrib.output test-hwloc-ls.output

noinst_HEADERS = misc.h

# Only build the utilities if we're building in standalone mode
if HWLOC_BUILD_UTILS
bin_PROGRAMS = lstopo hwloc-assembler hwloc-calc hwloc-bind hwloc-distances hwloc-distrib hwloc-ps
endif

lstopo_SOURCES = lstopo.h lstopo.c lstopo-color.c lstopo-text.c lstopo-draw.c lstopo-fig.c lstopo-xml.c
if HWLOC_HAVE_CAIRO
lstopo_SOURCES += lstopo-cairo.c
endif
if HWLOC_HAVE_WINDOWS
lstopo_SOURCES += lstopo-windows.c
endif
lstopo_CFLAGS = $(HWLOC_CAIRO_CFLAGS) $(HWLOC_LIBXML2_CFLAGS) $(HWLOC_PCI_CFLAGS)
lstopo_LDADD = $(LDADD) $(HWLOC_CAIRO_LIBS) -lm $(HWLOC_TERMCAP_LIBS) $(HWLOC_X11_LIBS)
if HWLOC_BUILD_UTILS
if HWLOC_HAVE_WINDOWS
bin_PROGRAMS += lstopo-win
lstopo_win_SOURCES = $(lstopo_SOURCES)
lstopo_win_CFLAGS = $(lstopo_CFLAGS) -mwindows
lstopo_win_LDADD = $(lstopo_LDADD)
endif
endif

hwloc_calc_SOURCES = hwloc-calc.c hwloc-calc.h

bin_SCRIPTS = hwloc-assembler-remote

# Only run the tests if we're building standalone, because the tests
# call hwloc executables.
if HWLOC_BUILD_TESTS
if !HWLOC_HAVE_MINGW32
TESTS = test-hwloc-assembler.sh test-hwloc-calc.sh test-hwloc-distances.sh test-hwloc-distrib.sh test-hwloc-ls.sh
endif !HWLOC_HAVE_MINGW32
endif HWLOC_BUILD_TESTS

# Only install man pages if we're building in standalone mode
if HWLOC_BUILD_UTILS
man7_pages = hwloc.7
EXTRA_DIST += $(man7_pages:.7=.7in)
nodist_man_MANS = $(man7_pages)

man1_pages = lstopo.1 hwloc-bind.1 hwloc-calc.1 hwloc-distances.1 hwloc-distrib.1 hwloc-ps.1 hwloc-assembler.1 hwloc-assembler-remote.1
EXTRA_DIST += $(man1_pages:.1=.1in)
nodist_man_MANS += $(man1_pages)

# Only install the gather-topology page if we're on Linux, but we need
# to include it in the tarball, regardless of what OS we're building
# on.
hgt_page = hwloc-gather-topology.1
EXTRA_DIST += $(hgt_page:.1=.1in)
if HWLOC_HAVE_LINUX
nodist_man_MANS += $(hgt_page)
endif HWLOC_HAVE_LINUX

.1in.1:
	@ echo Creating $@ man page...
	@ $(SED) -e 's/#PACKAGE_NAME#/@PACKAGE_NAME@/g' \
	  -e 's/#PACKAGE_VERSION#/@PACKAGE_VERSION@/g' \
	  -e 's/#HWLOC_DATE#/@HWLOC_RELEASE_DATE@/g' \
	  > $@ < $<

.3in.3:
	@ echo Creating $@ man page...
	@ $(SED) -e 's/#PACKAGE_NAME#/@PACKAGE_NAME@/g' \
	  -e 's/#PACKAGE_VERSION#/@PACKAGE_VERSION@/g' \
	  -e 's/#HWLOC_DATE#/@HWLOC_RELEASE_DATE@/g' \
	  > $@ < $<

.7in.7:
	@ echo Creating $@ man page...
	@ $(SED) -e 's/#PACKAGE_NAME#/@PACKAGE_NAME@/g' \
	  -e 's/#PACKAGE_VERSION#/@PACKAGE_VERSION@/g' \
	  -e 's/#HWLOC_DATE#/@HWLOC_RELEASE_DATE@/g' \
	  > $@ < $<

install-exec-hook: install-man
	rm -f $(DESTDIR)$(bindir)/hwloc-ls$(EXEEXT) $(DESTDIR)$(bindir)/hwloc-info$(EXEEXT) $(DESTDIR)$(bindir)/hwloc-mask$(EXEEXT)
	cd $(DESTDIR)$(bindir) && $(LN_S) lstopo$(EXEEXT) hwloc-ls$(EXEEXT) && $(LN_S) lstopo$(EXEEXT) hwloc-info$(EXEEXT) && $(LN_S) hwloc-calc$(EXEEXT) hwloc-mask$(EXEEXT)
	rm -f $(DESTDIR)$(man1dir)/hwloc-ls.1 $(DESTDIR)$(man1dir)/hwloc-info.1 $(DESTDIR)$(man1dir)/hwloc-mask.1
	cd $(DESTDIR)$(man1dir) && $(LN_S) lstopo.1 hwloc-ls.1 && $(LN_S) lstopo.1 hwloc-info.1 && $(LN_S) hwloc-calc.1 hwloc-mask.1
	$(SED) -e 's/abs_top_builddir\/utils/bindir/' -e '/abs_top_builddir/d' $(DESTDIR)$(bindir)/hwloc-assembler-remote > $(DESTDIR)$(bindir)/hwloc-assembler-remote.tmp && mv -f $(DESTDIR)$(bindir)/hwloc-assembler-remote.tmp $(DESTDIR)$(bindir)/hwloc-assembler-remote

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/hwloc-ls$(EXEEXT) $(DESTDIR)$(bindir)/hwloc-info$(EXEEXT) $(DESTDIR)$(bindir)/hwloc-mask$(EXEEXT)
	rm -f $(DESTDIR)$(man1dir)/hwloc-ls.1 $(DESTDIR)$(man1dir)/hwloc-info.1 $(DESTDIR)$(man1dir)/hwloc-mask.1

distclean-local:
	rm -f $(nodist_man_MANS)
endif
