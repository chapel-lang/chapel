#!/usr/bin/python
import sys,os,glob

files_in_order = [
    (0, "myth_malloc"),
    (0, "myth_free"),
    (0, "myth_calloc"),
    (0, "myth_posix_memalign"),
    (0, "myth_valloc"),
    (0, "myth_memalign"),
    (1, "myth_aligned_alloc"),
    (0, "myth_pvalloc"),
    (0, "myth_realloc"),
    (0, "myth_create_0"),
    (0, "myth_create_1"),
    (0, "myth_create_2"),
    (0, "myth_create_join_many"),
    (0, "myth_yield_0"),
    (0, "myth_yield_1"),
    (0, "myth_yield_2"),
    (0, "myth_sleep_queue"),
    (0, "myth_lock"),
    (0, "myth_trylock"),
    (0, "myth_mixlock"),
    (0, "myth_cond_signal"),
    (0, "myth_cond_broadcast_0"),
    (0, "myth_cond_broadcast_1"),
    (0, "myth_barrier"),
    (0, "myth_join_counter"),
    (0, "myth_felock"),
    (0, "myth_dag_1d"),
    (0, "myth_dag_2d"),
    (0, "myth_dag_random"),
    (0, "myth_key_create"),
    (0, "myth_key_getspecific"),
    (0, "myth_key_destructor"),
    (0, "measure_create"),
    (0, "measure_latency"),
    (0, "measure_wakeup_latency"),
    (0, "measure_malloc"),
    (0, "pth_barrier"),
    (0, "pth_cond_broadcast_0"),
    (0, "pth_cond_broadcast_1"),
    (0, "pth_cond_signal"),
    (0, "pth_create_0"),
    (0, "pth_create_1"),
    (0, "pth_create_2"),
    (0, "pth_lock"),
    (0, "pth_mixlock"),
    (0, "pth_mutex_initializer"),
    (0, "pth_trylock"),
    (0, "pth_yield"),
]

def file_order(f):
    for i,(c,x) in enumerate(files_in_order):
        if x in f:
            return i
    return len(files_in_order)

def build_condition(f):
    for i,(c,x) in enumerate(files_in_order):
        if x in f:
            if c:
                return "BUILD_TEST_%s" % x.upper()
            else:
                return None
    return None

def file_prefix(f):
    dot_pos = f.rindex(".")
    return f[:dot_pos]

def get_files(lib):
    C  = sorted([ f for f in glob.glob("*.c") ], key=file_order)
    CC = sorted([ f for f in glob.glob("*.cc") ], key=file_order)
    C_CC = C + CC
    if lib == "myth":
        return [ f for f in C_CC if not f.startswith("pth_") ]
    else:
        return C_CC

# wp = open("hoge.am", "wb")
wp = sys.stdout
wr = wp.write

wr("""
# this Makefile.am is generated automatically by gen_makefile_am.py
# when you like to add a new test, run: 
#    python gen_makefile_am.py > Makefile.am
#    cd ..
#    autoreconf

""")

wr("""
common_cflags = -I$(abs_top_srcdir)/include
common_cxxflags = $(common_cflags)
myth_ldadd = $(abs_top_srcdir)/src/libmyth.la
myth_ld_ldadd = $(abs_top_srcdir)/src/libmyth-ld.la
myth_dl_ldadd = $(abs_top_srcdir)/src/libmyth-dl.la -ldl
myth_ldflags = 
myth_ld_ldflags = @myth-ld.opts 
myth_dl_ldflags = 
""")

wr("""
check_PROGRAMS = 
""")

def add_check_programs(lib):
    assert lib.startswith("myth"), lib
    suffix = lib[4:]        #
    wr("\n")
    if suffix != "":
        wr("if BUILD_MYTH%s\n" % suffix.upper())
    for f in get_files(lib):
        cond = build_condition(f)
        base = file_prefix(f)
        base_suffix = "%s%s" % (base, suffix)
        if cond: wr("if %s\n" % cond)
        wr("check_PROGRAMS += %s\n" % base_suffix)
        if cond: wr("endif\n")
    if suffix != "":
        wr("endif\n")
            
def add_flags(lib):
    assert lib.startswith("myth"), lib
    suffix = lib[4:]        # 
    wr("\n")
    if suffix != "":
        wr("if BUILD_MYTH%s\n" % suffix.upper())
    for f in get_files(lib):
        cond = build_condition(f)
        base = file_prefix(f)
        base_suffix = "%s%s" % (base, suffix)
        if f.endswith(".c"):
            lang = "c"
        elif f.endswith(".cc"):
            lang = "cxx"
        else:
            assert 0, f
        if cond: wr("if %s\n" % cond)
        wr("""%s_SOURCES = %s
%s_%sFLAGS = $(common_%sflags)
%s_LDADD = $(%s_ldadd)
%s_LDFLAGS = $(%s_ldflags)
"""
           % (base_suffix, f,
              base_suffix, lang.upper(), lang,
              base_suffix, lib,
              base_suffix, lib))
        if cond: wr("endif\n")
    if suffix != "":
        wr("endif\n")

for lib in [ "myth", "myth_ld", "myth_dl" ]:
    add_check_programs(lib)

for lib in [ "myth", "myth_ld", "myth_dl" ]:
    add_flags(lib)

wr("""
TESTS = $(check_PROGRAMS)

build: $(check_PROGRAMS)
""")

if wp != sys.stdout:
    wp.close()

