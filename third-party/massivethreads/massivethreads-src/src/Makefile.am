# build this directory first and then tests
#SUBDIRS = . profiler
SUBDIRS = .

# sources (.c and .S) common across the three libraries
COMMON_SRCS = \
	myth_log.c \
	myth_sched.c \
	myth_worker.c \
	myth_sync.c \
	myth_init.c \
	myth_misc.c \
	myth_tls.c \
	myth_thread.c \
	myth_context.c \
	myth_context_switch.S \
	myth_eco.c \
	myth_if_native.c \
	myth_real.c

# sources for wrapping system functions (used only for -dl and -ld versions)
WRAP_SRCS = \
	myth_wrap_pthread.c \
	myth_wrap_malloc.c \
	myth_wrap_socket.c

COMMON_CFLAGS = \
	-D_GNU_SOURCE \
	-I$(abs_top_srcdir)/include

VANILLA_CFLAGS  = -DMYTH_WRAP=MYTH_WRAP_VANILLA
VANILLA_LDFLAGS = 
WRAP_LD_CFLAGS  = -DMYTH_WRAP=MYTH_WRAP_LD
WRAP_LD_LDFLAGS = @myth-ld.opts
WRAP_DL_CFLAGS  = -DMYTH_WRAP=MYTH_WRAP_DL
WRAP_DL_LDFLAGS = 

# required when you use libtool (for building shared libs)
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES =
lib_LTLIBRARIES += libmyth.la
if BUILD_MYTH_LD
lib_LTLIBRARIES += libmyth-ld.la
endif
if BUILD_MYTH_DL
lib_LTLIBRARIES += libmyth-dl.la 
endif

nobase_include_HEADERS = \
	mtbb/task_group.h \
	mtbb/parallel_for.h \
	mtbb/parallel_reduce.h \
	tpswitch/omp_dr.h \
	tpswitch/tbb_dr.h \
	tpswitch/cilkplus_dr.h \
	tpswitch/cilk_dr.cilkh \
	tpswitch/serial_dr.h \
	tpswitch/tpswitch.h

# myth_wrap_malloc.c myth_wrap_socket.c 

# vanilla
libmyth_la_SOURCES   = $(COMMON_SRCS) 
libmyth_la_CFLAGS    = $(COMMON_CFLAGS) $(VANILLA_CFLAGS)
libmyth_la_CCASFLAGS = $(COMMON_CFLAGS) $(VANILLA_CFLAGS)
libmyth_la_LDFLAGS   = $(VANILLA_LDFLAGS)

# wrap by ld --wrap 
if BUILD_MYTH_LD
libmyth_ld_la_SOURCES   = $(COMMON_SRCS) $(WRAP_SRCS)
libmyth_ld_la_CFLAGS    = $(COMMON_CFLAGS) $(WRAP_LD_CFLAGS)
libmyth_ld_la_CCASFLAGS = $(COMMON_CFLAGS) $(WRAP_LD_CFLAGS)
libmyth_ld_la_LDFLAGS   = $(WRAP_LD_LDFLAGS)
endif

# wrap by dlopen
if BUILD_MYTH_DL
libmyth_dl_la_SOURCES   = $(COMMON_SRCS) $(WRAP_SRCS)
libmyth_dl_la_CFLAGS    = $(COMMON_CFLAGS) $(WRAP_DL_CFLAGS)
libmyth_dl_la_CCASFLAGS = $(COMMON_CFLAGS) $(WRAP_DL_CFLAGS)
libmyth_dl_la_LDFLAGS   = $(WRAP_DL_LDFLAGS)
endif

