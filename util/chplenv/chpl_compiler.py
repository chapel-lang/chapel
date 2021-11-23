#!/usr/bin/env python3
import optparse
import os
import sys

from distutils.spawn import find_executable

import chpl_platform, chpl_locale_model, overrides
from utils import error, memoize, warning


#
# If we can't find a file $CHPL_HOME/make/Makefile.<compiler_val>,
# that suggests that this is a compiler that we're not familiar with.
# In practice, this will cause our Makefiles to use defaults like CC
# and CXX to compile things, for better or worse.
#
@memoize
def validate_compiler(compiler_val):
    if compiler_val != 'llvm':
        import chpl_home_utils
        chpl_home = chpl_home_utils.get_chpl_home()
        comp_makefile = os.path.join(chpl_home, 'make', 'compiler', 'Makefile.{0}'.format(compiler_val))
        if not os.path.isfile(comp_makefile):
            warning('Unknown compiler: "{0}"'.format(compiler_val))



@memoize
def get_prgenv_compiler():
    platform_val = chpl_platform.get('target')
    if platform_val.startswith('cray-x') or platform_val == 'hpe-cray-ex':
        subcompiler = os.environ.get('PE_ENV', 'none')
        if subcompiler != 'none':
            return "cray-prgenv-{0}".format(subcompiler.lower())

        else:
            warning("Compiling on {0} without a PrgEnv loaded".format(platform_val))

    return 'none'

# Don't use CC / CXX to set other variables if any of
#  CHPL_HOST_COMPILER
#  CHPL_HOST_CC
#  CHPL_HOST_CXX
#  CHPL_TARGET_COMPILER
#  CHPL_TARGET_CC
#  CHPL_TARGET_CXX
# are overridden by the user (in config file or env vars).
#
# Additionally, for the target compiler, don't use CC / CXX
# if we would like to default to LLVM.
@memoize
def should_consider_cc_cxx(flag):
    default_llvm = default_to_llvm(flag)
    if default_llvm:
        return False

    if (overrides.get('CHPL_HOST_COMPILER') != None or
        overrides.get('CHPL_HOST_CC') != None or
        overrides.get('CHPL_HOST_CXX') != None or
        overrides.get('CHPL_TARGET_COMPILER') != None or
        overrides.get('CHPL_TARGET_CC') != None or
        overrides.get('CHPL_TARGET_CXX') != None):
        # A compilation configuration setting was adjusted,
        # so require CHPL_HOST_CC etc rather than using CC
        return False

    if flag == 'target' and get_prgenv_compiler() != 'none':
        # On an XC etc with a PrgEnv compiler,
        # setting CC/CXX should only impact the host compiler.
        return False

    return True


# Figures out the compiler family (e.g. gnu) from the CC/CXX enviro vars
# Returns '' if CC / CXX are not set and 'unknown' if they are set
# to something too complex.
@memoize
def get_compiler_from_cc_cxx():
    cc_compiler = 'unknown'
    cxx_compiler = 'unknown'
    warn = False
    compiler_val = 'unknown'
    cc_val = overrides.get('CC', '')
    cxx_val = overrides.get('CXX', '')

    if cc_val == '' and cxx_val == '':
        return ''

    if cc_val:
        cc_compiler = get_compiler_from_command(cc_val)
    if cxx_val:
        cxx_compiler = get_compiler_from_command(cxx_val)

    if cc_val and cxx_val:
        if cc_compiler == cxx_compiler:
            compiler_val = cc_compiler
        else:
            error("Conflicting compiler families for CC and CXX settings\n"
                  "  {0} -> {1}\n"
                  "  {2} -> {3}\n"
                  "Set CHPL_HOST_COMPILER and CHPL_TARGET_COMPILER to the "
                  "desired compiler family".format(cc_val, cc_compiler,
                      cxx_val, cxx_compiler))
            compiler_val = 'unknown'

    else:
        # if we get here, CC or CXX is provided, but not both.
        # Usually we warn in that case.
        # Check to see if the command name matches the default
        # for the compiler family.
        # In that event, omit the warning.
        if cc_val:
            compiler_val = cc_compiler
            warn = (get_compiler_name_c(compiler_val) != cc_val)
        if cxx_val:
            compiler_val = cxx_compiler
            warn = (get_compiler_name_cxx(compiler_val) != cxx_val)

    if compiler_val == 'unknown':
        error("Could not infer CHPL_TARGET_COMPILER from "
              "CC={0} CXX={1}".format(cc_val, cxx_val));
    else:
        if warn and cc_val:
            error('CC is set but not CXX -- please set both\n')
        if warn and cxx_val:
            error('CXX is set but not CC -- please set both\n')

    return compiler_val

# Returns True if the compiler defaults to LLVM
def default_to_llvm(flag):
    ret = False

    if flag == 'target':
        import chpl_llvm
        has_llvm = chpl_llvm.get()

        if has_llvm == 'bundled' or has_llvm  == 'system':
            # Default to CHPL_TARGET_COMPILER=llvm when CHPL_LLVM!=none
            ret = True

    return ret

@memoize
def get(flag='host'):

    if flag == 'host':
        compiler_val = overrides.get('CHPL_HOST_COMPILER', '')

    elif flag == 'target':
        compiler_val = overrides.get('CHPL_TARGET_COMPILER', '')

    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    default_llvm = False
    if not compiler_val:
        default_llvm = default_to_llvm(flag)

        # If allowable, look at CC/CXX
        if should_consider_cc_cxx(flag):
            compiler_val = get_compiler_from_cc_cxx()

    if compiler_val:
        validate_compiler(compiler_val)
        return compiler_val

    prgenv_compiler = get_prgenv_compiler()

    if default_llvm:
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
        locale_model_val = chpl_locale_model.get()
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
        elif locale_model_val == 'gpu':
            if find_executable('clang'):
                compiler_val = 'clang'
            else:
                error("clang not found. The 'gpu' locale model is supported "
                      "with clang only.")
        else:
            compiler_val = 'gnu'

    validate_compiler(compiler_val)
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

# This array consists of tuples of
#  ( family-name, c-compilation-command-name, c++compilation-command-name)
# where family-name corresponds to CHPL_TARGET_COMPILER etc settings e.g. gnu.
# This table only includes the cases where it is reasonable to
# infer the family from the command name.
COMPILERS = [ ('gnu', 'gcc', 'g++'),
              ('clang', 'clang', 'clang++'),
              ('ibm', 'xlc', 'xlC'),
              ('intel', 'icc', 'icpc'),
              ('pgi', 'pgicc', 'pgc++') ]

# given a compiler command, (e.g. gcc or /path/to/clang++),
# figure out the compiler family (e.g. gnu or clang),
# and the C and C++ variants of that command
def get_compiler_from_command(command):
    # the following adjustments are to handle a command like
    #    /path/to/gcc-10 --some-option
    # where we are looking for just the 'gcc' part.
    first = command.split()[0]
    basename = os.path.basename(first)
    name = basename.split('-')[0]
    for tup in COMPILERS:
        if name == tup[1] or name == tup[2]:
            return tup[0]

    # if it was not one of the above cases we don't know how to
    # go from the command name to the compiler family.
    # E.g. cc/CC/mpicc could be many compilers.
    #
    # We could consider trying to run it to figure it out.
    return 'unknown'

def get_compiler_name_c(compiler):
    for tup in COMPILERS:
        if compiler == tup[0]:
            return tup[1]

    # handle special cases not in the COMPILERS table
    if compiler_is_prgenv(compiler):
        return 'cc'
    elif compiler in ['llvm', 'allinea']:
        return 'clang'
    elif compiler == 'mpi-gnu':
        return 'mpicc'
    elif 'gnu' in compiler:
        return 'gcc'

    return 'unknown-c-compiler'

def get_compiler_name_cxx(compiler):
    for tup in COMPILERS:
        if compiler == tup[0]:
            return tup[2]

    # handle special cases not in the COMPILERS table
    if compiler_is_prgenv(compiler):
        return 'CC'
    elif compiler in ['llvm', 'allinea']:
        return 'clang++'
    elif compiler == 'mpi-gnu':
        return 'mpicxx'
    elif 'gnu' in compiler:
        return 'g++'

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
    elif lang_upper == 'C':
        lang_upper = 'CC'

    if flag_upper == 'HOST' or flag_upper == 'TARGET':
        pass
    else:
        error('unknown flag {0}'.format(flag))

    if lang_upper == 'CC' or lang_upper == 'CXX':
        pass
    else:
        error('unknown lang {0}'.format(lang))

    # construct CHPL_HOST_CC / CHPL_TARGET_CXX etc
    varname = 'CHPL_' + flag_upper + '_' + lang_upper

    command = overrides.get(varname, '');
    if command:
        return command

    compiler_val = get(flag=flag)

    # If other settings allow it, look also at CC/CXX.
    if should_consider_cc_cxx(flag):
        cc_cxx_val = overrides.get(lang_upper, '')
        if cc_cxx_val:
            return cc_cxx_val

    if lang_upper == 'CC':
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

def validate_inference_matches(flag, lang):
    flag_upper = flag.upper()
    lang_upper = lang.upper()

    if lang_upper == 'C++':
        lang_upper = 'CXX'
    elif lang_upper == 'C':
        lang_upper = 'CC'

    compiler = get(flag)
    cmd = get_compiler_command(flag, lang)
    inferred = get_compiler_from_command(cmd)

    if (inferred != 'unknown' and
        inferred != compiler and
        not (compiler == 'llvm' and inferred == 'clang')):
        error("Conflicting compiler families: "
              "CHPL_{0}_COMPILER={1} but CHPL_{0}_{2}={3} but has family {4}"
              .format(flag_upper, compiler, lang_upper, cmd, inferred))
        return False

    return True

# Issue an error if, after all the various inferences are done,
# CHPL_HOST_CC / CXX is inconsintent with CHPL_HOST_COMPILER
# and similarly for TARGET variants.
@memoize
def validate_compiler_settings():
    validate_inference_matches('host', 'c')
    validate_inference_matches('host', 'c++')
    validate_inference_matches('target', 'c')
    validate_inference_matches('target', 'c++')

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
