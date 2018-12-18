#!/usr/bin/env python
import collections
import optparse
import os
import platform
from string import punctuation
from sys import stderr, stdout

import chpl_comm, chpl_compiler, chpl_platform, overrides
from compiler_utils import CompVersion, compiler_is_prgenv, get_compiler_version
from utils import memoize, run_command


#
# Intel architectures are accessed with the -march= flag.
# ARM architectures are accessed with the -mcpu= flag, except that some
# compilers can handle -march=native for ARM (and some cannot).
# Therefore, our translations need to specify which flag is used.
#
class argument_map(object):
    # intel does not support amd archs... it may be worth testing setting the
    # equivalent intel arch, but I don't have any good way to do this as of now
    intel = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'corei7',
        'westmere':      'corei7',
        'sandybridge':   'corei7-avx',
        'ivybridge':     'core-avx-i',
        'haswell':       'core-avx2',
        'broadwell':     'core-avx2',
        'knc':           'knc',
        'mic-knl':       'knl',
        'x86-skylake':   'skylake-avx512',
        'k8':            'none',
        'k8sse3':        'none',
        'barcelona':     'none',
        'bdver1':        'none',
        'bdver2':        'none',
        'bdver3':        'none',
        'bdver4':        'none',
        'aarch64':       'none',
        'arm-thunderx':  'none',
        'arm-thunderx2': 'none',
    }

    gcc43 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'core2',
        'westmere':      'core2',
        'sandybridge':   'core2',
        'ivybridge':     'core2',
        'haswell':       'core2',
        'broadwell':     'core2',
        'knc':           'none',
        'mic-knl':       'none',
        'x86-skylake':   'none',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'barcelona',
        'bdver2':        'barcelona',
        'bdver3':        'barcelona',
        'bdver4':        'barcelona',
        'aarch64':       'none',
        'arm-thunderx':  'none',
        'arm-thunderx2': 'none',
    }

    gcc47 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'corei7',
        'westmere':      'corei7',
        'sandybridge':   'corei7-avx',
        'ivybridge':     'core-avx-i',
        'haswell':       'core-avx2',
        'broadwell':     'core-avx2',
        'knc':           'none',
        'mic-knl':       'none',
        'x86-skylake':   'none',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver2',
        'bdver4':        'bdver2',
        'aarch64':       'none',
        'arm-thunderx':  'none',
        'arm-thunderx2': 'none',
    }

    gcc48 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'corei7',
        'westmere':      'corei7',
        'sandybridge':   'corei7-avx',
        'ivybridge':     'core-avx-i',
        'haswell':       'core-avx2',
        'broadwell':     'core-avx2',
        'knc':           'none',
        'mic-knl':       'none',
        'x86-skylake':   'none',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver2',
        'bdver4':        'bdver2',
        'aarch64':       'generic',
        'arm-thunderx':  'generic',
        'arm-thunderx2': 'generic',
    }

    gcc49 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'nehalem',
        'westmere':      'westmere',
        'sandybridge':   'sandybridge',
        'ivybridge':     'ivybridge',
        'haswell':       'haswell',
        'broadwell':     'broadwell',
        'knc':           'none',
        'mic-knl':       'none',
        'x86-skylake':   'none',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver3',
        'bdver4':        'bdver4',
        'aarch64':       'generic',
        'arm-thunderx':  'generic',
        'arm-thunderx2': 'generic',
    }

    gcc5 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'nehalem',
        'westmere':      'westmere',
        'sandybridge':   'sandybridge',
        'ivybridge':     'ivybridge',
        'haswell':       'haswell',
        'broadwell':     'broadwell',
        'knc':           'none',
        'mic-knl':       'knl',
        'x86-skylake':   'none',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver3',
        'bdver4':        'bdver4',
        'aarch64':       'generic',
        'arm-thunderx':  'generic',
        'arm-thunderx2': 'generic',
    }

    gcc6 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'nehalem',
        'westmere':      'westmere',
        'sandybridge':   'sandybridge',
        'ivybridge':     'ivybridge',
        'haswell':       'haswell',
        'broadwell':     'broadwell',
        'knc':           'none',
        'mic-knl':       'knl',
        'x86-skylake':   'skylake-avx512',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver3',
        'bdver4':        'bdver4',
        'aarch64':       'thunderx',
        'arm-thunderx':  'thunderx',
        'arm-thunderx2': 'thunderx',
    }

    gcc7 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'nehalem',
        'westmere':      'westmere',
        'sandybridge':   'sandybridge',
        'ivybridge':     'ivybridge',
        'haswell':       'haswell',
        'broadwell':     'broadwell',
        'knc':           'none',
        'mic-knl':       'knl',
        'x86-skylake':   'skylake-avx512',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver3',
        'bdver4':        'bdver4',
        'aarch64':       'thunderx',
        'arm-thunderx':  'thunderx',
        'arm-thunderx2': 'thunderx2t99',
    }

    gcc8 = {
        'native':        'native',
        'core2':         'core2',
        'nehalem':       'nehalem',
        'westmere':      'westmere',
        'sandybridge':   'sandybridge',
        'ivybridge':     'ivybridge',
        'haswell':       'haswell',
        'broadwell':     'broadwell',
        'knc':           'none',
        'mic-knl':       'knl',
        'x86-skylake':   'skylake-avx512',
        'k8':            'k8',
        'k8sse3':        'k8-sse3',
        'barcelona':     'barcelona',
        'bdver1':        'bdver1',
        'bdver2':        'bdver2',
        'bdver3':        'bdver3',
        'bdver4':        'bdver4',
        'aarch64':       'thunderx',
        'arm-thunderx':  'thunderx',
        'arm-thunderx2': 'thunderx2t99',
    }

    clang = gcc8

    @classmethod
    def find(cls, arch, compiler, version):
        if arch == 'unknown' or arch == '':
            return 'unknown'
        elif arch == 'none':
            return 'none'

        arg_value = cls._get(arch, compiler, version)
        if not arg_value:
            stderr.write('Warning: No valid option found: arch="{0}" '
                         'compiler="{1}" version="{2}"\n'.format(arch,
                                                                 compiler,
                                                                 version))
        return arg_value or None

    @classmethod
    def _get(cls, arch, compiler, version):
        if arch == 'unknown':
            return arch

        if compiler in ['gnu', 'mpi-gnu', 'cray-prgenv-gnu']:
            if version >= CompVersion('8.0'):
                return cls.gcc8.get(arch, '')
            elif version >= CompVersion('7.0'):
                return cls.gcc7.get(arch, '')
            elif version >= CompVersion('6.0'):
                return cls.gcc6.get(arch, '')
            elif version >= CompVersion('5.0'):
                return cls.gcc6.get(arch, '')
            elif version >= CompVersion('4.9'):
                return cls.gcc49.get(arch, '')
            elif version >= CompVersion('4.8'):
                return cls.gcc48.get(arch, '')
            elif version >= CompVersion('4.7'):
                return cls.gcc47.get(arch, '')
            elif version >= CompVersion('4.3'):
                return cls.gcc43.get(arch, '')
            return 'none'
        elif compiler in ['intel', 'cray-prgenv-intel']:
            return cls.intel.get(arch, '')
        elif compiler in ['clang', 'clang-included', 'allinea']:
            # Clang doesn't know how to do architecture detection for aarch64.
            if arch == 'native':
                if get_native_machine() == 'aarch64':
                    return 'unknown'
            return cls.clang.get(arch, '')
        else:
            stderr.write('Warning: Unknown compiler: "{0}"\n'.format(compiler))
            return ''


class feature_sets(object):
    core2 = ['mmx', 'sse', 'sse2', 'ssse3']
    nehalem = core2 + ['sse41', 'sse42', 'popcnt']
    westmere = nehalem + ['aes', 'pclmulqdq']
    sandybridge = westmere + ['avx']
    ivybridge = sandybridge + ['rdrand', 'f16c']
    haswell = ivybridge + ['movbe', 'avx2', 'fma', 'bmi1', 'bmi2']
    broadwell = haswell + ['rdseed', 'adx']
    knl = broadwell + ['avx512f', 'avx512cd',
                       'avx512er', 'avx512pf']
    skylake = broadwell + ['avx512f', 'avx512cd',
                           'avx512bw', 'avx512dq', 'avx512vl']

    intel = [
        ('core2',       core2),
        ('nehalem',     nehalem),
        ('westmere',    westmere),
        ('sandybridge', sandybridge),
        ('ivybridge',   ivybridge),
        ('haswell',     haswell),
        ('broadwell',   broadwell),
        ('mic-knl',     knl),
        ('x86-skylake', skylake),
    ]

    k8 = ['mmx', 'sse', 'sse2']
    k8sse3 = k8 + ['sse3']
    barcelona = k8sse3 + ['sse4a', 'abm']
    bdver1 = barcelona + ['fma4', 'avx', 'xop', 'lwp', 'aes',
                          'pclmul', 'cx16', 'sse41', 'sse42']
    bdver2 = bdver1 + ['bmi', 'tbm', 'f16c', 'fma']
    bdver3 = bdver2 + ['fsgsbase']
    bdver4 = bdver3 + ['bmi2', 'avx2', 'movbe']

    amd = [
        ('k8',        k8),
        ('k8sse3',    k8sse3),
        ('barcelona', barcelona),
        ('bdver1',    bdver1),
        ('bdver2',    bdver2),
        ('bdver3',    bdver3),
        ('bdver4',    bdver4),
    ]

    thunderx = ['fp', 'asimd', 'evtstrm', 'aes', 'pmull',
                'sha1', 'sha2', 'crc32']
    thunderx2 = thunderx + ['atomics']

    arm = [
        ('aarch64',       thunderx),
        ('arm-thunderx',  thunderx),
        ('arm-thunderx2', thunderx2),
    ]

    combined = intel + amd + arm

    @classmethod
    def subset(sets, a, b):
        def check(lst, a, b):
            def list_in(x, y):
                if len(x) > len(y):
                    return False

                for val in x:
                    if val not in y:
                        return False
                return True

            a_features = []
            b_features = []
            for k, v in lst:
                if k == a:
                    a_features = v
                if k == b:
                    b_features = v
            return a_features != [] and list_in(a_features, b_features)

        return check(sets.combined, a, b)


    @classmethod
    def find(sets, vendor, features):
        def remove_punctuation(s):
            exclude = set(punctuation)
            return  ''.join(ch for ch in s if ch not in exclude)

        # remove all punctuation and split into a list
        system_features = remove_punctuation(features.lower()).split()

        options = []
        if "genuineintel" == vendor.lower():
            options = sets.intel
        elif "authenticamd" == vendor.lower():
            options = sets.amd
        elif "arm" == vendor.lower():
            options = sets.arm

        found = ''
        for name, fset in options:
            if all([f in system_features for f in fset]):
                found = name
            else:
                break

        return found

# This gets the generic machine type, e.g. x86_64, i686, aarch64.
# Since uname returns the host machine type, we currently assume that
# cross-compilation is limited to different subarchitectures of the
# generic machine type.  For example, we can cross compile from
# sandybridge to broadwell, but not x86_64 to aarch64.
@memoize
def get_native_machine():
    return platform.uname()[4]

@memoize
def is_known_arm(arch):
    if arch.startswith("arm-") or ('aarch64' in arch) or ('thunderx' in arch):
        return True
    else:
        return False

# Take a machine name in the same format as uname -m and answer
# whether or not it is in the x86 family.
@memoize
def is_x86_variant(machine):
    if machine.startswith('i') and machine.endswith('86'):  # e.g. i686
        return True
    if machine in ['amd64', 'x64', 'x86', 'x86-64', 'x86_64']:
        return True
    return False

def get_cpuinfo(platform_val='linux'):
    vendor_string = ''
    feature_string = ''
    if platform_val == "darwin":
        vendor_string = run_command(['sysctl', '-n', 'machdep.cpu.vendor'])
        feature_string = run_command(['sysctl', '-n', 'machdep.cpu.features'])
        # osx reports AVX1.0 while linux reports it as AVX
        feature_string = feature_string.replace("AVX1.0", "AVX")
        feature_string = feature_string.replace("SSE4.", "SSE4")
    elif os.path.isfile('/proc/cpuinfo'):
        with open('/proc/cpuinfo') as f:
            cpuinfo = f.readlines()
        # Compensate for missing vendor in ARM /proc/cpuinfo
        if get_native_machine() == 'aarch64':
            vendor_string = "arm"
        for line in cpuinfo:
            if 'vendor_id' in line:
                vendor_string = line.split(':')[1].strip()
            elif 'flags' in line:
                feature_string = line.split(':')[1].strip()
            elif line.startswith('Features'):
                feature_string = line.split(':')[1].strip()
            if vendor_string and feature_string:
                feature_string = feature_string.replace("sse4_", "sse4")
                break
    else:
        raise ValueError("Unknown platform, could not find CPU information")
    return (vendor_string.strip(), feature_string.strip())

class InvalidLocationError(ValueError):
    pass

# We build the module with the lowest common denominator cpu architecture for
# each platform. This is so that end users can build programs with any
# compatible cpu architecture loaded and we don't have to build a binary for
# each one. However, we put the cpu architecture that the module was built with
# in the gen directory so we need a way to get the proper path no matter what
# cpu architecture is actually loaded. Note that this MUST be kept in sync with
# what we have in the module build script.
def get_module_lcd_arch(platform_val, arch):
    if platform_val == "cray-xc":
        if is_known_arm(arch):
            return "arm-thunderx2"
        else:
            return "sandybridge"
    elif platform_val == "cray-xe" or platform_val == "cray-xk":
        return "barcelona"
    elif platform_val == "aarch64":
        return "arm-thunderx"
    else:
        return 'none'

# Adjust the architecture based upon compiler support
@memoize
def adjust_architecture_for_compiler(arch, flag, get_lcd):
    compiler_val = chpl_compiler.get(flag)
    platform_val = chpl_platform.get(flag)

    isprgenv = compiler_is_prgenv(compiler_val)
    if compiler_val == 'clang-included':
      isprgenv = compiler_is_prgenv(chpl_compiler.get(flag,
                                                      llvm_mode="orig"))

    if isprgenv:
        cray_arch = os.environ.get('CRAY_CPU_TARGET', 'none')
        if arch and (arch != 'none' and arch != 'unknown' and arch != cray_arch):
            stderr.write("Warning: Setting the processor type through "
                         "environment variables is not supported for "
                         "cray-prgenv-*. Please use the appropriate craype-* "
                         "module for your processor type.\n")
        arch = cray_arch
        if arch == 'none':
            stderr.write("Warning: No craype-* processor type module was "
                         "detected, please load the appropriate one if you want "
                         "any specialization to occur.\n")
        if get_lcd:
            arch = get_module_lcd_arch(platform_val, arch)
            if arch == 'none':
                stderr.write("Warning: Could not detect the lowest common "
                             "denominator processor type for this platform. "
                             "You may be unable to use the Chapel compiler\n")
        return arch
    elif 'pgi' in compiler_val:
        return 'none'
    elif 'cray' in compiler_val:
        return 'none'
    elif 'ibm' in compiler_val:
        return 'none'

    return arch

@memoize
def default_arch(flag):
    comm_val = chpl_comm.get()
    compiler_val = chpl_compiler.get(flag)
    platform_val = chpl_platform.get(flag)

    arch = 'unknown'

    if comm_val == 'none' and ('linux' in platform_val or
                               platform_val == 'darwin' or
                               platform_val.startswith('cygwin')):
      # Clang cannot detect the architecture for aarch64.  Otherwise,
      # let the backend compiler do the actual feature set detection. We
      # could be more aggressive in setting a precise architecture using
      # the double checking code above, but it seems like a waste of time
      # to not use the work the backend compilers have already done
      if compiler_val in ['clang', 'clang-included']:
          if get_native_machine() == 'aarch64':
              arch = 'unknown'
          else:
              arch = 'native'
      else:
            arch = 'native'

    return arch


# Given an arch, raise an error if it's not a reasonable setting
@memoize
def verify_arch(arch, flag):
    comm_val = chpl_comm.get()
    compiler_val = chpl_compiler.get(flag)
    platform_val = chpl_platform.get(flag)
    isprgenv = compiler_is_prgenv(compiler_val)

    # Only try to do any architecture verification when:
    # comm == none  -- The inverse means that we are probably cross-compiling.
    #
    # linux/dawin/  -- The only platforms that we should try and detect on.
    # cygwin           Crays will be handled through the craype-* modules
    #
    check_arch = False
    if not isprgenv:
        if flag == 'target':
            if comm_val == 'none':
                if ('linux' in platform_val or
                     platform_val == 'darwin' or
                     platform_val.startswith('cygwin')):
                    check_arch = True
        if flag == 'host':
            check_arch = True

    if check_arch and arch and arch not in  ['none', 'unknown', 'native']:
        # Print a friendly warning if it's unlikely the user could run
        # their program. This could be printed in cross-compilation settings.
        warn = False
        try:
            vendor_string, feature_string = get_cpuinfo(platform_val)
            detected_arch = feature_sets.find(vendor_string, feature_string)
            if not feature_sets.subset(arch, detected_arch):
                warn = True
        except ValueError:
            stderr.write("Warning: Unknown platform, could not find CPU information\n")

        if warn:
                stderr.write("Warning: The supplied processor type does "
                             "not appear to be compatible with the host "
                             "processor type. The resultant binary may "
                             "not run on the current machine.\n")

# get_lcd has no effect on non cray systems and is intended to be used to get
# the correct runtime and gen directory.
@memoize
def get(flag, map_to_compiler=False, get_lcd=False):

    arch_tuple = collections.namedtuple('arch_tuple', ['flag', 'arch'])

    if not flag or flag == 'host':
        arch = overrides.get('CHPL_HOST_ARCH', '')
    elif flag == 'target':
        arch = overrides.get('CHPL_TARGET_ARCH', '')
    else:
        raise InvalidLocationError(flag)

    # fast path out for when the user has set arch=none
    if arch == 'none' or (flag == 'host' and not arch):
        return arch_tuple('none', 'none')

    # Adjust arch for compiler (not all compilers support all arch
    # settings; PrgEnv might override arch, etc)
    arch = adjust_architecture_for_compiler (arch, flag, get_lcd)

    # Now, if is not yet set, we should set the default.
    if not arch:
        arch = default_arch(flag)

    verify_arch(arch, flag)

    compiler_val = chpl_compiler.get(flag)
    isprgenv = compiler_is_prgenv(compiler_val)
    if map_to_compiler and not isprgenv:
        # Map arch to compiler argument
        # Don't do this for PrgEnv compiles since the compiler driver
        # handles specialization.
        version = get_compiler_version(compiler_val)
        arch = argument_map.find(arch, compiler_val, version)
    if arch and arch != 'none' and arch != 'unknown':
        # x86 uses -march= where the others use -mcpu=
        if is_x86_variant(get_native_machine()):
            flag = 'arch'
        else:
            flag = 'cpu'
    else:
        flag = 'none'

    return arch_tuple(flag or 'none', arch or 'unknown')


# Returns the default machine.  The flag argument is 'host' or 'target'.
#
# Note that we cannot deduce the machine type corresponding to a given
# architecture.  For example, knowing that we are on a sandybridge does
# not tell us whether we are running in 32-bit mode (sometimes designated
# as i686) or 64-bit mode (x86_64).  For some architectures, the same is
# true of big-endian vs. little-endian mode as well.  Consequently, we have
# insufficient information about the target.  Therefore, the machine type must
# be the same for host and target, though the subarchitecture can be different.
# We can cross compile from sandybridge to broadwell but not from
# x86_64 to aarch64.
#
# Because of all this, the flag argument is unused and we return the same
# answer for host or target.
@memoize
def get_default_machine(flag):
    return get_native_machine()


def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target] "
                                         "[--compflag] [--lcdflag] "
                                         "[--specialize]")
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host')
    parser.add_option('--comparch', dest='map_to_compiler',
                      action='store_true', default=False)
    parser.add_option('--compflag', dest='compflag', action='store_true',
                      default=False)
    parser.add_option('--lcdflag', dest = 'get_lcd', action='store_true',
                      default=False)
    (options, args) = parser.parse_args()

    (flag, arch) = get(options.flag, options.map_to_compiler,
                       options.get_lcd)

    if options.compflag:
        stdout.write("{0}=".format(flag))
    stdout.write("{0}\n".format(arch))

if __name__ == '__main__':
    _main()
