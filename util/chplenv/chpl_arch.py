#!/usr/bin/env python

import os, optparse
from sys import stdout, stderr
from string import punctuation

import utils, chpl_platform, chpl_comm, chpl_compiler
from utils import memoize

class argument_map(object):
    # intel does not support amd archs... it may be worth testing setting the
    # equivalent intel arch, but I don't have any good way to do this as of now
    intel = {
        'native':      'native',
        'core2':       'core2',
        'nehalem':     'corei7',
        'westmere':    'corei7',
        'sandybridge': 'corei7-avx',
        'ivybridge':   'core-avx-i',
        'haswell':     'core-avx2',
        'broadwell':   'core-avx2',
        'knc':         'knc',
        'k8':          'none',
        'k8sse3':      'none',
        'barcelona':   'none',
        'bdver1':      'none',
        'bdver2':      'none',
        'bdver3':      'none',
        'bdver4':      'none',
    }

    gcc43 = {
        'native':      'native',
        'core2':       'core2',
        'nehalem':     'core2',
        'westmere':    'core2',
        'sandybridge': 'core2',
        'ivybridge':   'core2',
        'haswell':     'core2',
        'broadwell':   'core2',
        'knc':         'none',
        'k8':          'k8',
        'k8sse3':      'k8-sse3',
        'barcelona':   'barcelona',
        'bdver1':      'barcelona',
        'bdver2':      'barcelona',
        'bdver3':      'barcelona',
        'bdver4':      'barcelona',
    }

    gcc47 = {
        'native':      'native',
        'core2':       'core2',
        'nehalem':     'corei7',
        'westmere':    'corei7',
        'sandybridge': 'corei7-avx',
        'ivybridge':   'core-avx-i',
        'haswell':     'core-avx2',
        'broadwell':   'core-avx2',
        'knc':         'none',
        'k8':          'k8',
        'k8sse3':      'k8-sse3',
        'barcelona':   'barcelona',
        'bdver1':      'bdver1',
        'bdver2':      'bdver2',
        'bdver3':      'bdver2',
        'bdver4':      'bdver2',
    }

    gcc49 = {
        'native':      'native',
        'core2':       'core2',
        'nehalem':     'nehalem',
        'westmere':    'westmere',
        'sandybridge': 'sandybridge',
        'ivybridge':   'ivybridge',
        'haswell':     'haswell',
        'broadwell':   'broadwell',
        'knc':         'none',
        'k8':          'k8',
        'k8sse3':      'k8-sse3',
        'barcelona':   'barcelona',
        'bdver1':      'bdver1',
        'bdver2':      'bdver2',
        'bdver3':      'bdver3',
        'bdver4':      'bdver4',
    }

    clang = gcc47

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
        return arg_value

    @classmethod
    def _get(cls, arch, compiler, version):
        if arch == 'unknown':
            return arch

        if compiler == 'gnu':
            if version.major > 4:
                return cls.gcc49.get(arch, '')
            elif version.major == 4:
                if version.minor >= 9:
                    return cls.gcc49.get(arch, '')
                elif version.minor >= 7:
                    return cls.gcc47.get(arch, '')
                elif version.minor >= 3:
                    return cls.gcc43.get(arch, '')
            return 'none'
        elif compiler == 'intel':
            return cls.intel.get(arch, '')
        elif compiler == 'clang':
            return cls.clang.get(arch, '')
        elif compiler == 'clang-included':
            return cls.clang.get(arch, '')
        else:
            stderr.write('Warning: Unknown compiler: "{0}"\n'.format(compiler))
            return ''


class feature_sets(object):
    core2 = ['mmx', 'sse', 'sse2', 'sse3', 'ssse3']
    nehalem = core2 + ['sse41', 'sse42', 'popcnt']
    westmere = nehalem + ['aes', 'pclmulqdq']
    sandybridge = westmere + ['avx']
    ivybridge = sandybridge + ['rdrand', 'f16c']
    haswell = ivybridge + ['movbe', 'avx2', 'fma', 'bmi', 'bmi2']
    broadwell = haswell + ['rdseed', 'adx']

    intel = [
        ('core2',       core2),
        ('nehalem',     nehalem),
        ('westmere',    westmere),
        ('sandybridge', sandybridge),
        ('ivybridge',   ivybridge),
        ('haswell',     haswell),
        ('broadwell',   broadwell),
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

    combined = intel + amd

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
        # remove all punctuation and split into a list
        system_features = features.lower().translate(None, punctuation).split()

        options = []
        if "genuineintel" == vendor.lower():
            options = sets.intel
        elif "authenticamd" == vendor.lower():
            options = sets.amd

        found = ''
        for name, fset in options:
            if all([f in system_features for f in fset]):
                found = name
            else:
                break

        return found

def get_cpuinfo(platform='linux'):
    vendor_string = ''
    feature_string = ''
    if platform == "darwin":
        vendor_string = utils.run_command(['sysctl',
                                           '-n',
                                           'machdep.cpu.vendor'])
        feature_string = utils.run_command(['sysctl',
                                            '-n',
                                            'machdep.cpu.features'])
        # osx reports AVX1.0 while linux reports it as AVX
        feature_string = feature_string.replace("AVX1.0", "AVX")
    elif os.path.isfile('/proc/cpuinfo'):
        with open('/proc/cpuinfo') as f:
            cpuinfo = f.readlines()
        for line in cpuinfo:
            if 'vendor_id' in line:
                vendor_string = line.split(':')[1].strip()
            elif 'flags' in line:
                feature_string = line.split(':')[1].strip()
            if vendor_string and feature_string:
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
    if arch == 'knc':
        return arch

    if platform_val == "cray-xc":
        return "sandybridge"
    elif platform_val == "cray-xe" or platform_val == "cray-xk":
        return "barcelona"
    else:
        return 'none'

# get_lcd has no affect on non cray systems and is intended to be used to get
# the correct runtime and gen directory.
@memoize
def get(location, map_to_compiler=False, get_lcd=False):

    if not location or location == "host":
        arch = os.environ.get('CHPL_HOST_ARCH', '')
    elif location == 'target':
        arch = os.environ.get('CHPL_TARGET_ARCH', '')
    else:
        raise InvalidLocationError(location)

    # fast path out for when the user has set arch=none
    if arch == 'none':
        return arch

    comm_val = chpl_comm.get()
    compiler_val = chpl_compiler.get(location)
    platform_val = chpl_platform.get(location)

    isprgenv = utils.compiler_is_prgenv(compiler_val)

    if isprgenv:
        if arch and (arch != 'none' or arch != 'unknown'):
            stderr.write("Warning: Setting the processor type through "
                         "environment variables is not supported for "
                         "cray-prgenv-*. Please use the appropriate craype-* "
                         "module for your processor type.\n")
        arch = os.environ.get('CRAY_CPU_TARGET', 'none')
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

    # Only try to do any auto-detection or verification when:
    # comm == none  -- The inverse means that we are probably cross-compiling.
    #
    # linux/dawin/  -- The only platforms that we should try and detect on.
    # cygwin           Crays will be handled through the craype-* modules
    #
    if comm_val == 'none' and ('linux' in platform_val or
                               platform_val == 'darwin' or
                               platform_val.startswith('cygwin')):
        if arch:
            if arch != 'knc' and not location or location == 'host':
                # when a user supplies an architecture, and it seems reasonable
                # to double check their choice we do so. This will only
                # generate a warning that the user may not be able to run
                # whatever they compile.
                #
                # This only runs when location is 'host' since we
                # conservatively assume that a setting for 'target' could be in
                # a cross-compilation setting
                try:
                    vendor_string, feature_string = get_cpuinfo(platform_val)
                    detected_arch = feature_sets.find(vendor_string, feature_string)
                    if not feature_sets.subset(arch, detected_arch):
                        stderr.write("Warning: The supplied processor type does "
                                     "not appear to be compatible with the host "
                                     "processor type. The resultant binary may "
                                     "not run on the current machine.\n")
                except ValueError:
                    stderr.write("Warning: Unknown platform, could not find CPU information\n")
        else:
            # let the backend compiler do the actual feature set detection. We
            # could be more aggressive in setting a precise architecture using
            # the double checking code above, but it seems like a waste of time
            # to not use the work the backend compilers have already done
            arch = 'native'


    if map_to_compiler:
        version = utils.get_compiler_version(compiler_val)
        arch = argument_map.find(arch, compiler_val, version)

    return arch or 'unknown'


def _main():
    parser = optparse.OptionParser(usage="usage: %prog [--host|target] "
                                         "[--compflag] [--lcdflag]")
    parser.add_option('--target', dest='location', action='store_const',
                      const='target', default='target')
    parser.add_option('--host', dest='location', action='store_const',
                      const='host')
    parser.add_option('--compflag', dest='map_to_compiler', action='store_true',
                      default=False)
    parser.add_option('--lcdflag', dest = 'get_lcd', action='store_true',
                      default=False)
    (options, args) = parser.parse_args()

    arch = get(options.location, options.map_to_compiler, options.get_lcd)

    stdout.write("{0}\n".format(arch))


if __name__ == '__main__':
    _main()
