#!/usr/bin/env python3
import optparse
import os
import sys

from distutils.spawn import find_executable

import chpl_platform, overrides
from utils import error, memoize


#
# If we can't find a file $CHPL_HOME/make/Makefile.<compiler_val>,
# that suggests that this is a compiler that we're not familiar with.
# In practice, this will cause our Makefiles to use defaults like CC
# and CXX to compile things, for better or worse.
#
@memoize
def validate(compiler_val):
    if compiler_val != 'llvm':
        import chpl_home_utils
        chpl_home = chpl_home_utils.get_chpl_home()
        comp_makefile = os.path.join(chpl_home, 'make', 'compiler', 'Makefile.{0}'.format(compiler_val))
        if not os.path.isfile(comp_makefile):
            sys.stderr.write('Warning: Unknown compiler: "{0}"\n'.format(compiler_val))



@memoize
def get_prgenv_compiler():
    platform_val = chpl_platform.get('target')
    if platform_val.startswith('cray-x') or platform_val == 'hpe-cray-ex':
        subcompiler = os.environ.get('PE_ENV', 'none')
        if subcompiler != 'none':
            return "cray-prgenv-{0}".format(subcompiler.lower())

        else:
            sys.stderr.write("Warning: Compiling on {0} without a PrgEnv loaded\n".format(platform_val))

    return 'none'

# Figures out the compiler family (e.g. gnu) from the CC/CXX enviro vars
@memoize
def get_compiler_from_cc_cxx():
    cc_val = overrides.get('CC', '')
    cxx_val = overrides.get('CXX', '')
    if cc_val and cxx_val:
        cc_val_command = get_compiler_from_command(cc_val)
        cxx_val_command = get_compiler_from_command(cxx_val)
        if (cc_val_command != cxx_val_command):
            error("Conflicting compiler families for CC and CXX settings\n"
                  "  {0} -> {1}\n"
                  "  {2} -> {3}".format(cc_val, cc_val_command,
                                        cxx_val, cxx_val_command))
    elif cc_val and not cxx_val:
        sys.stderr.write('Warning: CC is set but not CXX -- please set both\n')
    elif cxx_val and not cc_val:
        sys.stderr.write('Warning: CXX is set but not CC -- please set both\n')

    compiler_val = ''
    if cc_val:
        compiler_val = get_compiler_from_command(cc_val)
    if cxx_val:
        compiler_val = get_compiler_from_command(cxx_val)
    return compiler_val

@memoize
def get(flag='host'):

    if flag == 'host':
        compiler_val = overrides.get('CHPL_HOST_COMPILER', '')

    elif flag == 'target':
        compiler_val = overrides.get('CHPL_TARGET_COMPILER', '')

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    # If compiler_val was not set, look at CC/CXX
    if not compiler_val:
        compiler_val = get_compiler_from_cc_cxx()

    if compiler_val:
        validate(compiler_val)
        return compiler_val

    prgenv_compiler = get_prgenv_compiler()
    has_llvm = 'unset'

    if flag == 'target':
        import chpl_llvm
        has_llvm = chpl_llvm.get()

    if flag == 'target' and (has_llvm == 'bundled' or has_llvm  == 'system'):
        # Default to CHPL_TARGET_COMPILER=llvm when CHPL_LLVM!=none
        compiler_val = 'llvm'

    elif prgenv_compiler != 'none':
        # The cray platforms are a special case in that we want to
        # "cross-compile" by default. (the compiler is different between host
        # and target, but the platform is the same).
        if flag == 'host':
            compiler_val = 'gnu'
        else:
            compiler_val = prgenv_compiler

    else:
        platform_val = chpl_platform.get(flag)
        # Normal compilation (not "cross-compiling")
        # inherit the host compiler if the target compiler is not set and
        # the host and target platforms are the same
        if flag == 'target':
            if chpl_platform.get('host') == platform_val:
                compiler_val = get('host')
        elif platform_val.startswith('pwr'):
            compiler_val = 'ibm'
        elif platform_val == 'darwin' or platform_val == 'freebsd':
            if find_executable('clang'):
                compiler_val = 'clang'
            else:
                compiler_val = 'gnu'
        else:
            compiler_val = 'gnu'

    validate(compiler_val)
    return compiler_val

@memoize
def get_path_component(flag='host'):

    val = get(flag=flag)

    if val == 'clang' and flag == 'target':

        import chpl_llvm
        has_llvm = chpl_llvm.get()

        if has_llvm == 'bundled':
            # selecting the included clang - distinguish that
            # with 'llvm' in the path component.
            val = 'llvm'

    return get(flag)

# given a compiler command, (e.g. gcc or /path/to/clang++),
# figure out the compiler family (e.g. gnu or clang)
def get_compiler_from_command(command):
    # get the basename after splitting on whitespace
    # e.g. to handle a command like
    #    /path/to/gcc --some-option
    name = os.path.basename(command.split()[0])
    if name == 'gcc' or name == 'g++':
        return 'gnu'
    elif name == 'clang' or name == 'clang++':
        return 'clang'
    elif name == 'xlc' or name == 'xlC':
        return 'ibm'
    elif name == 'icc' or name == 'icpc':
        return 'intel'
    elif name == 'pgcc' or name == 'pgc++':
        return 'pgc++'

    # if it was not one of the above cases we don't know how to
    # go from the command name to the compiler family.
    # E.g. cc/CC/mpicc could be many compilers.
    #
    # We could consider trying to run it to figure it out.
    return 'unknown'

def get_compiler_name_c(compiler):
    if compiler_is_prgenv(compiler):
        return 'cc'
    elif 'gnu' in compiler:
        return 'gcc'
    elif compiler in ['clang', 'llvm', 'allinea']:
        return 'clang'
    elif compiler == 'ibm':
        return 'xlc'
    elif compiler == 'intel':
        return 'icc'
    elif compiler == 'pgi':
        return 'pgcc'
    elif compiler == 'mpi-gnu':
        return 'mpicc'

    return 'unknown-c-compiler'

def get_compiler_name_cxx(compiler):
    if compiler_is_prgenv(compiler):
        return 'CC'
    elif 'gnu' in compiler:
        return 'g++'
    elif compiler in ['clang', 'llvm', 'allinea']:
        return 'clang++'
    elif compiler == 'ibm':
        return 'xlC'
    elif compiler == 'intel':
        return 'icpc'
    elif compiler == 'pgi':
        return 'pgc++'
    elif compiler == 'mpi-gnu':
        return 'mpicxx'

    return 'unknown-c++-compiler'

def compiler_is_prgenv(compiler_val):
  return compiler_val.startswith('cray-prgenv')

# flag should be host or target
# lang should be c or cxx (aka c++)
@memoize
def get_compiler_command(flag, lang):

    flag_upper = flag.upper()
    lang_upper = lang.upper()

    if lang_upper == 'C++':
        lang_upper = 'CXX'

    if flag_upper == 'HOST' or flag_upper == 'TARGET':
        pass
    else:
        error('unknown flag {0}'.format(flag))

    if lang_upper == 'C' or lang_upper == 'CXX':
        pass
    else:
        error('unknown lang {0}'.format(lang))

    varname = 'CHPL_' + flag_upper + '_COMPILER_COMMAND_' + lang_upper

    command = overrides.get(varname, '');
    if command:
        return command

    # If CHPL_TARGET_COMPILER_COMMAND_C etc was not set, look at CC/CXX
    if lang_upper == 'C':
        cc_val = overrides.get('CC', '')
        if cc_val:
            return cc_val

    if lang_upper == 'CXX':
        cxx_val = overrides.get('CXX', '')
        if cxx_val:
            return cxx_val

    compiler_val = get(flag=flag)

    if lang_upper == 'C':
        command = get_compiler_name_c(compiler_val)
    elif lang_upper == 'CXX':
        command = get_compiler_name_cxx(compiler_val)

    # Adjust the path in two situations:
    #  CHPL_TARGET_COMPILER=llvm -- means use the selected llvm/clang
    #  CHPL_TARGET_COMPILER=clang with CHPL_LLVM=bundled -- use bundled clang
    if compiler_val == 'clang' or compiler_val == 'llvm':
        import chpl_llvm
        llvm_val = chpl_llvm.get()
        if llvm_val == 'bundled' or compiler_val == 'llvm':
            if (flag == 'host' and
                llvm_val == 'bundled' and
                compiler_val == 'clang'):
                # don't change the prefix in this setting
                # (bundled LLVM might not be built yet)
                pass

            else:
                command = chpl_llvm.get_llvm_clang(lang_upper)

    return command

def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host', default='host')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target')
    (options, args) = parser.parse_args()

    compiler_val = get(options.flag)
    sys.stdout.write("{0}\n".format(compiler_val))


if __name__ == '__main__':
    _main()
