import chpl_compiler, chpl_comm_debug, chpl_comm_segment, chpl_comm_substrate
import overrides, chpl_home_utils, compiler_utils, third_party_utils
from utils import error, warning, memoize
import os

@memoize
def get_uniq_cfg_path():
    base_uniq_cfg = third_party_utils.default_uniq_cfg_path()
    if chpl_comm_debug.get() == 'debug':
        base_uniq_cfg += '-debug'
    substrate = chpl_comm_substrate.get()
    segment = chpl_comm_segment.get()
    return base_uniq_cfg + '/substrate-' + substrate + '/seg-' + segment


@memoize
def get_gasnet_pc_file():
    substrate = chpl_comm_substrate.get()
    pcfile = "gasnet-{0}-par.pc".format(substrate)
    return pcfile

# If we are using a PrgEnv environment, gasnet is built with
# PrgEnv-gnu even when CHPL_TARGET_COMPILER is llvm.
# The gasnet .pc file includes some GCC options in that event.
# This function filters these out.
#
# This is a workaround.
# TODO: Do we still need to switch to PrgEnv-gnu when building gasnet?
def filter_compile_args(args):
    compiler = chpl_compiler.get('target')
    is_prgenv = compiler_utils.target_compiler_is_prgenv(bypass_llvm=True)

    ret = [ ]
    if compiler == 'llvm' and is_prgenv:
        # filter out compile arguments not starting with -D or -I
        n = len(args)
        i = 0
        while i < n:
            s = args[i]
            if s.startswith('-D') or s.startswith('-I'):
                ret.append(s)
            if len(s) == 2 and i+1 < n:
                # if it was just -D or -I, add the next argument too
                i += 1
                ret.append(args[i])
            i += 1
        return ret
    else:
        # otherwise, just return the args the way they were
        ret = args

    # subsequently, filter away a few gasnet flags we don't want
    # an alternative to this filtering would be to only grab
    # certain flags from the gasnet .pc file
    more_filtered = [ ]
    for arg in ret:
        if arg.startswith('-O') or arg == '-Winline' or arg == '-g':
            pass # leave out this flag
        else:
            more_filtered.append(arg)

    return more_filtered

# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    tup = third_party_utils.pkgconfig_get_bundled_compile_args(
                       'gasnet', get_uniq_cfg_path(), get_gasnet_pc_file())
    return (filter_compile_args(tup[0]), filter_compile_args(tup[1]))

# Filter link args to avoid -W arguments that can't be handled by ld
# and also to avoid -O arguments.
def filter_link_args(args):
    ret = [ ]
    for arg in args:
        if arg.startswith('-O') or arg.startswith('-W') or arg == '-g':
            pass # leave out this flag
        else:
            ret.append(arg)

    return ret

# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    tup = third_party_utils.pkgconfig_get_bundled_link_args(
                       'gasnet', get_uniq_cfg_path(), get_gasnet_pc_file())
    return (filter_link_args(tup[0]), filter_link_args(tup[1]))

# returns the special linker to use, or None if there is none
# (e.g. it might link with mpicc)
@memoize
def get_override_ld():
    (d, _) = third_party_utils.read_bundled_pkg_config_file(
                       'gasnet', get_uniq_cfg_path(), get_gasnet_pc_file())

    if d == None:
        return None # no ld override

    if 'GASNET_LD' in d:
        gasnet_ld = d['GASNET_LD']
        gasnet_cc = d['GASNET_CC']
        gasnet_cxx = d['GASNET_CXX']
        gasnet_ld_requires_mpi = d.get('GASNET_LD_REQUIRES_MPI', False)

        ld = None # no ld override

        mpi_cxx = overrides.get('MPI_CXX')
        if not mpi_cxx:
            mpi_cxx = 'mpicxx'

        target_compiler_llvm = False
        if chpl_compiler.get('target') == 'llvm':
            target_compiler_llvm = True

        # For the linker, we tend to want a C++ linker, in order to support
        # linking in components developed in C++ like re2 or user C++ code.
        # However, GASNet doesn't provide any guarantees that their linker
        # will be a C++ compiler.  Based on conversation with the GASNet
        # team, we should expect it either to be GASNET_CC, GASNET_CXX, or
        # MPI_CC.  The following conditional handles these cases, switching
        # to the C++ choice in the first and third cases.
        if gasnet_ld == gasnet_cxx:
            if not target_compiler_llvm:
                ld = gasnet_ld # GASNet chose C++ linker so stick with it
        elif gasnet_ld == gasnet_cc:
            if not target_compiler_llvm:
                ld = gasnet_cxx # GASNet chose C, so switch to C++
        elif gasnet_ld_requires_mpi:
            ld = mpi_cxx
        else:
            warning("GASNET_LD unexpectedly set to '{0}'. "
                    "Please file a Chapel bug against this.".format(gasnet_ld))
            ld = None

        return ld

    return None
