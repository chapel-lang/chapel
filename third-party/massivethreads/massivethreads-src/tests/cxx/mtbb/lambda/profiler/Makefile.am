check_PROGRAMS = lambda_profiler_serial lambda_profiler_mth

lambda_profiler_serial_SOURCES = lambda_profiler.cc
lambda_profiler_mth_SOURCES    = lambda_profiler.cc

lambda_profiler_serial_CXXFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -std=c++0x -DTO_SERIAL
lambda_profiler_mth_CXXFLAGS    = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -std=c++0x -DTO_MTHREAD_NATIVE

lambda_profiler_serial_LDADD = $(abs_top_srcdir)/src/profiler/libdr.la 
lambda_profiler_mth_LDADD    = $(abs_top_srcdir)/src/profiler/libdr.la $(abs_top_srcdir)/src/libmyth.la

TESTS = $(check_PROGRAMS)
