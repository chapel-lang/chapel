import os
from chplenv import *

# Given two 2-tuples of lists, add 2nd lists to the first lists
def extend2(x, y):
    x[0].extend(y[0])
    x[1].extend(y[1])

# Remove duplicates, keeping last occurrence and preserving order
# e.g. "-lhwloc -lqthread -lhwloc ..." -> "-lqthread -lhwloc ...
def dedup(args):
    seen = set()
    ret = [arg for arg in reversed(args)
           if not (arg in seen or seen.add(arg))]
    return reversed(ret)

# Returns the runtime includes and defines according
# to the current configuration, for a target (not host) compile.
# Returns tuple of (bundled_args, system_args)
def get_runtime_includes_and_defines():
    bundled = [ ]
    system = [ ]

    incl = chpl_home_utils.get_chpl_runtime_incl()
    locale_model = chpl_locale_model.get()
    comm = chpl_comm.get();
    tasks = chpl_tasks.get()
    atomics = chpl_atomics.get()
    mem = chpl_mem.get('target')
    third_party = chpl_home_utils.get_chpl_third_party()
    platform = chpl_platform.get('target')

    bundled.append("-I" + os.path.join(incl, "localeModels", locale_model))
    bundled.append("-I" + os.path.join(incl, "localeModels"))
    bundled.append("-I" + os.path.join(incl, "comm", comm))
    bundled.append("-I" + os.path.join(incl, "comm"))
    bundled.append("-I" + os.path.join(incl, "tasks", tasks))
    bundled.append("-I" + incl)
    bundled.append("-I" + os.path.join(incl, "qio"))
    bundled.append("-I" + os.path.join(incl, "atomics", atomics))
    bundled.append("-I" + os.path.join(incl, "mem", mem))
    bundled.append("-I" + os.path.join(incl, "mem", mem))
    bundled.append("-I" + os.path.join(third_party, "utf8-decoder"))

    if platform.startswith("cygwin"):
        # w32api is provided by cygwin32-w32api-runtime
        system.append("-I" + os.path.join("usr", "include", "w32api"))

    if locale_model == "gpu":
        # this -D is needed since it affects code inside of headers
        bundled.append("-DHAS_GPU_LOCALE")
        # If compiling for GPU locales, add CUDA runtime headers to include path
        cuda_path = chpl_gpu.get_cuda_path()
        system.append("-I" + os.path.join(cuda_path, "include"))
        bundled.append("-I" + os.path.join(incl, "gpu", chpl_gpu.get()))

    if mem == "jemalloc":
        # set -DCHPL_JEMALLOC_PREFIX=chpl_je_
        # this is needed since it affects code inside of headers
        bundled.append("-DCHPL_JEMALLOC_PREFIX=chpl_je_")

    return (bundled, system)

# Returns the runtime -L and -l args according
# to the current configuration, for a target (not host) compile.
# Returns tuple of (bundled_args, system_args)
def get_runtime_link_args(runtime_subdir):
    bundled = [ ]
    system = [ ]

    lib = chpl_home_utils.get_chpl_runtime_lib()
    locale_model = chpl_locale_model.get()

    bundled.append("-L" + os.path.join(lib, runtime_subdir))
    bundled.append("-lchpl")

    if locale_model == "gpu":
        # If compiling for GPU locales, add CUDA to link path,
        # and add cuda libraries
        cuda_path = chpl_gpu.get_cuda_path()
        system.append("-L" + os.path.join(cuda_path, "lib64"))
        system.append("-lcuda")
        system.append("-lcudart")

    # always link with the math library
    system.append("-lm")
    # always link with the pthreads library
    system.append("-lpthread")

    return (bundled, system)

# Returns a dictionary containing keys
#   'host_compile'
#   'host_link'
#   'target_compile'
#   'target_link'
#
# where each value is a tuple of compile/link args
#  (bundled_args, system_args)
def compute_internal_compile_link_args(runtime_subdir):
    # compute the compiler / link args
    # each of these is bundled, system
    host_compile = ([ ], [ ])
    host_link = ([ ], [ ])
    tgt_compile = ([ ], [ ])
    tgt_link = ([ ], [ ])

    skip_host = os.environ.get('CHPLENV_SKIP_HOST', None)

    # add runtime includes and defines
    extend2(tgt_compile, get_runtime_includes_and_defines())
    extend2(tgt_link, get_runtime_link_args(runtime_subdir))

    # add 3p arguments

    if (chpl_llvm.get() == 'bundled' or
        chpl_llvm.get() == 'system'):
        if not skip_host:
            extend2(host_compile, chpl_llvm.get_host_compile_args())
            extend2(host_link, chpl_llvm.get_host_link_args())

    extend2(tgt_compile, chpl_gmp.get_compile_args())
    extend2(tgt_link, chpl_gmp.get_link_args())

    extend2(tgt_compile, chpl_hwloc.get_compile_args())
    extend2(tgt_link, chpl_hwloc.get_link_args())

    if chpl_comm.get() == 'ofi':
        extend2(tgt_compile, chpl_libfabric.get_compile_args())
        extend2(tgt_link, chpl_libfabric.get_link_args())
    elif chpl_comm.get() == 'gasnet':
        extend2(tgt_compile, chpl_gasnet.get_compile_args())
        extend2(tgt_link, chpl_gasnet.get_link_args())
    elif chpl_comm.get() == 'ugni':
        # If there isn't a hugepage module loaded, we need to request
        # libhugetlbfs ourselves.
        pe_product_list = os.environ.get('PE_PRODUCT_LIST', None)
        if pe_product_list and 'HUGETLB' in pe_product_list:
            tgt_link[1].append('-lhugetlbfs')

    if chpl_tasks.get() == 'qthreads':
        extend2(tgt_compile, chpl_qthreads.get_compile_args())
        extend2(tgt_link, chpl_qthreads.get_link_args())

    extend2(tgt_compile, chpl_unwind.get_compile_args())
    extend2(tgt_link, chpl_unwind.get_link_args())

    extend2(tgt_compile, chpl_jemalloc.get_compile_args('target'))
    extend2(tgt_link, chpl_jemalloc.get_link_args('target'))
    if not skip_host:
        extend2(host_compile, chpl_jemalloc.get_compile_args('host'))
        extend2(host_link, chpl_jemalloc.get_link_args('host'))

    if chpl_re2.get() != 'none':
        extend2(tgt_compile, chpl_re2.get_compile_args())
        extend2(tgt_link, chpl_re2.get_link_args())

    aux_filesys = chpl_aux_filesys.get()
    if 'lustre' in aux_filesys:
        tgt_compile[1].append('-DSYS_HAS_LLAPI')
        tgt_link[1].append('-llustreapi')
    if 'hdfs' in aux_filesys:
        java_install = os.environ.get('JAVA_INSTALL', None)
        hadoop_install = os.environ.get('HADOOP_INSTALL', None)
        if java_install:
            java_include = os.path.join(java_install, 'include')
            tgt_compile[1].append('-I' + java_include)
            tgt_compile[1].append('-I' + os.path.join(java_include, 'linux'))
            java_lib = os.path.join(java_install, 'lib', 'amd64', 'server')
            tgt_link[1].append('-L' + java_lib)
        if hadoop_install:
            hadoop_include = os.path.join(hadoop_install, 'include')
            tgt_compile[1].append('-I' + hadoop_include)
            hadoop_lib = os.path.join(hadoop_install, 'lib', 'native')
            tgt_link[1].append('-L' + hadoop_lib)

    # add arguments indicated by compiler selection
    # (e.g. we might have a -I above for a particular LLVM version,
    #  so we want that version before /usr/local/include/llvm.)
    if not skip_host:
        host_compile[0].extend(chpl_compiler.get_bundled_compile_args('host'))
        host_compile[1].extend(chpl_compiler.get_system_compile_args('host'))
        host_link[0].extend(chpl_compiler.get_bundled_link_args('host'))
        host_link[1].extend(chpl_compiler.get_system_link_args('host'))

    tgt_compile[0].extend(chpl_compiler.get_bundled_compile_args('target'))
    tgt_compile[1].extend(chpl_compiler.get_system_compile_args('target'))
    tgt_link[0].extend(chpl_compiler.get_bundled_link_args('target'))
    tgt_link[1].extend(chpl_compiler.get_system_link_args('target'))


    # remove duplicate system libraries
    host_link = (host_link[0], dedup(host_link[1]))
    tgt_link = (tgt_link[0], dedup(tgt_link[1]))

    return {'host_compile': host_compile,
            'host_link': host_link,
            'target_compile': tgt_compile,
            'target_link': tgt_link}
