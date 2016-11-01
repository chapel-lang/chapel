#noinst_PROGRAMS = bin_serial bin_mth_native bin_mth_compat
check_PROGRAMS = bin_serial bin_mth_native bin_mth_compat
bin_serial_SOURCES     = bin.cc
bin_mth_native_SOURCES = bin.cc
bin_mth_compat_SOURCES = bin.cc

bin_serial_CXXFLAGS     = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -std=c++0x -DTO_SERIAL
bin_mth_native_CXXFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -std=c++0x -DTO_MTHREAD_NATIVE
bin_mth_compat_CXXFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -std=c++0x -DTO_MTHREAD

bin_mth_native_LDADD = $(abs_top_srcdir)/src/libmyth.la 
bin_mth_compat_LDADD = $(abs_top_srcdir)/src/libmyth.la 

TESTS = $(check_PROGRAMS)
