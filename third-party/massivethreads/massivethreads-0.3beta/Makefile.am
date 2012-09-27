# build this directory first and then tests
SUBDIRS = . tests

# sources (.c and .S) and flags common across the three libraries
COMMON_SRCS = myth_log.c myth_sched.c myth_worker.c myth_malloc_wrapper.c myth_sync.c myth_init.c myth_misc.c myth_io.c myth_original_lib.c myth_tls.c myth_context.S myth_context_gvar.c
COMMON_FLAGS = -D_GNU_SOURCE 

# required when you use libtool (for building shared libs)
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libmyth.la libmyth-compat.la libmyth-native.la
include_HEADERS = myth.h myth_if_native.h 
nobase_include_HEADERS = mtbb/task_group.h

libmyth_la_SOURCES        = $(COMMON_SRCS) myth_if_native.c myth_if_pthread.c myth_constructor.c
libmyth_native_la_SOURCES = $(COMMON_SRCS) myth_if_native.c
libmyth_compat_la_SOURCES = $(COMMON_SRCS)                  myth_if_pthread.c myth_constructor.c

# C compiler flags
libmyth_la_CFLAGS         = $(COMMON_FLAGS)
libmyth_native_la_CFLAGS  = $(COMMON_FLAGS)
libmyth_compat_la_CFLAGS  = $(COMMON_FLAGS)

# this is for assembly (.S) file = myth_context.S
libmyth_la_CCASFLAGS      = $(COMMON_FLAGS)
libmyth_native_la_CCASFLAGS  = $(COMMON_FLAGS)
libmyth_compat_la_CCASFLAGS  = $(COMMON_FLAGS)

pthread_so_path.def:
	$(CC) -o search_shlib_path search_shlib_path.c -lpthread
	ldd search_shlib_path | grep libpthread.so | $(AWK) '{printf("#define LIBPTHREAD_PATH \"%s\"", $$3); }' > pthread_so_path.def
	rm search_shlib_path

myth.d: pthread_so_path.def
	gcc -MM -w *.c > myth.d

-include myth.d

