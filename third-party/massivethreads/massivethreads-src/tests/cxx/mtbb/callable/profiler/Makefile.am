check_PROGRAMS = callable_profiler_serial callable_profiler_mth

callable_profiler_serial_SOURCES = callable_profiler.cc
callable_profiler_mth_SOURCES    = callable_profiler.cc

callable_profiler_serial_CXXFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -DTO_SERIAL
callable_profiler_mth_CXXFLAGS    = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -I$(abs_top_srcdir)/src/mtbb -DTO_MTHREAD_NATIVE

callable_profiler_serial_LDADD = $(abs_top_srcdir)/src/profiler/libdr.la
callable_profiler_mth_LDADD    = $(abs_top_srcdir)/src/profiler/libdr.la $(abs_top_srcdir)/src/libmyth.la 

TESTS = $(check_PROGRAMS)
