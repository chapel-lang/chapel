#!/usr/bin/env python

import os, re, subprocess, sys, string, optparse
#sys.stdout.write("native")
#sys.exit(0)

# very simple logger (should be easy to replace with a real system if needed)
class Logger(object):
    enabled = False

    def _log(self, fmt, msg, *args):
        if self.enabled:
            sys.stderr.write(fmt.format(msg % args))

    def warn(self, msg, *args):
        self._log("Warning: {0}\n", msg, *args)

    def error(self, msg, *args):
        self._log("Error: {0}\n", msg, *args)

logger = Logger()

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
            logger.warn('No valid option found: arch="%s" compiler="%s" version="%s"',
                        arch, compiler, version)
        return arg_value

    @classmethod
    def _get(cls, arch, compiler, version):
        if arch == 'unknown':
            return arch

        if compiler == 'gnu':
            if version >= '4.9':
                return cls.gcc49.get(arch, '')
            if version >= '4.7':
                return cls.gcc47.get(arch, '')
            if version >= '4.3':
                return cls.gcc43.get(arch, '')
            else:
                logger.warn('Argument map not found for GCC version: "%s"', version)
                return ''
        elif compiler == 'intel':
            return cls.intel.get(arch, '')
        elif compiler == 'clang':
            return cls.clang.get(arch, '')
        else:
            logger.warn('Unknown compiler: "%s"', compiler)
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

    k8 =  ['mmx', 'sse', 'sse2']
    k8sse3 =  k8 + ['sse3']
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
            return (a_features != [] and list_in(a_features, b_features))

        return check(sets.combined, a, b)


    @classmethod
    def find(sets, vendor, features):
        # remove all punctuation and split into a list
        system_features = features.lower().translate(None, string.punctuation).split()

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

class CommandError(Exception):
    pass

# This could be replaced by subprocess.check_output, but that isn't available
# until python 2.7 and we only have 2.6 on most machines :(
def run_command(command, stdout=True, stderr=False):
    process = subprocess.Popen(command,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()
    if process.returncode != 0:
        raise CommandError("command {0} failed - output was \n{1}".format(command, output[1]))
    else:
        if stdout and stderr:
            return output
        elif stdout:
            return output[0]
        elif stderr:
            return output[1]
        else:
            return ''

def get_cpuinfo(platform='linux'):
    vendor_string = ''
    feature_string = ''
    if platform == "darwin":
        vendor_string = run_command(['sysctl',
                                      '-n',
                                      'machdep.cpu.vendor'])
        feature_string = run_command(['sysctl',
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

def get_compiler_version(compiler):
    if compiler == 'gnu':
        output = run_command(['gcc', '-dumpversion'])
        match = re.search(r'(\d+\.\d+)', output)
        if match:
            return match.group(1)
        else:
            raise ValueError("Could not find the GCC version")
    else:
        return ''

class InvalidLocationError(ValueError):
    pass

def get_arch_flag(location, map_to_compiler=False):

    if not location or location == "host":
        arch = os.environ.get('CHPL_HOST_ARCH', '')
    elif location == 'target':
        arch = os.environ.get('CHPL_TARGET_ARCH', '')
    else:
        raise InvalidLocationError(location)

    # fast path out for when the user as set arch=none
    if arch == 'none':
        return arch

    chpl_home = os.environ.get('CHPL_HOME', '')
    if not chpl_home:
        dirname = os.path.dirname
        chpl_home  = dirname(dirname(dirname(os.path.realpath(__file__))))
        logger.warn('CHPL_HOME not set, assuming "%s"', chpl_home)

    comm_path = os.path.join(chpl_home, 'util', 'chplenv', 'comm')
    comm = run_command([comm_path, '--'+location]).strip()

    compiler_path = os.path.join(chpl_home, 'util', 'chplenv', 'compiler')
    compiler = run_command([compiler_path, '--'+location]).strip()

    platform_path = os.path.join(chpl_home, 'util', 'chplenv', 'platform')
    platform = run_command([platform_path, '--'+location]).strip()

    if 'cray-prgenv' in compiler:
        if arch and (arch != 'none' or arch != 'unknown'):
            logger.warn("Setting the processor type through environment variables "
                        "is not supported for cray-prgenv-*. Please use the "
                        "appropriate craype-* module for your processor type.")
        arch = os.environ.get('CRAY_CPU_TARGET', 'none')
        if arch == 'none':
            logger.warn("No craype-* processor type module was detected, please "
                        "load the appropriate one if you want any specialization "
                        "to occur.")
        return arch
    elif 'pgi' in compiler:
        return 'none'
    elif 'cray' in compiler:
        return 'none'
    elif 'ibm' in compiler:
        return 'none'

    # Only try to do any auto-detection or verification when:
    # comm == none  -- The inverse means that we are probably cross-compiling.
    #
    # linux/dawin/  -- The only platforms that we should try and detect on.
    # cygwin           Crays will be handled through the craype-* modules
    #
    if comm == 'none' and ('linux' in platform or platform == 'darwin' or platform == 'cygwin'):
        if arch:
            if not location or location == 'host':
                # when a user supplies an architecture, and it seems reasonable
                # to double check their choice we do so. This will only
                # generate a warning that the user may not be able to run
                # whatever they compile.
                #
                # This only runs when location is 'host' since we
                # conservatively assume that a setting for 'target' could be in
                # a cross-compilation setting
                try:
                    vendor_string, feature_string = get_cpuinfo(platform)
                    detected_arch = feature_sets.find(vendor_string, feature_string)
                    if not feature_sets.subset(arch, detected_arch):
                        logger.warn("The supplied processor type does not "
                                    "appear to be compatible with the host "
                                    "processor type. The resultant binary may "
                                    "not run on the current machine.")
                except ValueError:
                    logger.warn("Unknown platform, could not find CPU information")
        else:
            # let the backend compiler do the actual feature set detection. We
            # could be more aggressive in setting a precise architecture using
            # the double checking code above, but it seems like a waste of time
            # to not use the work the backend compilers have already done
            arch = 'native'

    if map_to_compiler:
        version = get_compiler_version(compiler)
        arch = argument_map.find(arch, compiler, version)

    return arch


if __name__ == '__main__':
    logger.enabled = True
    usage = "usage: arch.py [--host|target] [--compflag]\n"

    location = ''
    map_to_compiler = False
    if len(sys.argv) >= 2:
        for arg in sys.argv[1:]:
            if arg == '--host' or arg == '--target':
                location = arg[2:]
            elif arg == '--compflag':
                map_to_compiler = True
            else:
                sys.stderr.write("Error: Unknown argument: {0}\n".format(arg))
                sys.stderr.write(usage)
                sys.exit(1)

    arch = get_arch_flag(location, map_to_compiler)

    if not arch:
        arch = "unknown"
    sys.stdout.write("{0}\n".format(arch))
    sys.exit(0)
