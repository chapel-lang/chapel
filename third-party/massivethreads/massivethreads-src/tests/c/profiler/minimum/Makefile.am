check_PROGRAMS = \
	minimum_tbblike_standalone \
	minimum_tbblike_mth \
	minimum_omplike_standalone \
	minimum_omplike_mth

minimum_tbblike_standalone_SOURCES = minimum_tbblike.c
minimum_tbblike_mth_SOURCES        = minimum_tbblike.c
minimum_omplike_standalone_SOURCES = minimum_omplike.c
minimum_omplike_mth_SOURCES        = minimum_omplike.c

minimum_tbblike_standalone_CFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -DSTANDALONE_TEST
minimum_tbblike_mth_CFLAGS        = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler
minimum_omplike_standalone_CFLAGS = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler -DSTANDALONE_TEST
minimum_omplike_mth_CFLAGS        = -I$(abs_top_srcdir)/include -I$(abs_top_srcdir)/src/profiler 

minimum_tbblike_standalone_LDADD = $(abs_top_srcdir)/src/profiler/libdr.la
minimum_tbblike_mth_LDADD        = $(abs_top_srcdir)/src/profiler/libdr.la $(abs_top_srcdir)/src/libmyth.la
minimum_omplike_standalone_LDADD = $(abs_top_srcdir)/src/profiler/libdr.la
minimum_omplike_mth_LDADD        = $(abs_top_srcdir)/src/profiler/libdr.la $(abs_top_srcdir)/src/libmyth.la

TESTS = $(check_PROGRAMS)
