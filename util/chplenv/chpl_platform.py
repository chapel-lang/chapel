#!/usr/bin/env python3
import optparse
import os
import platform
import re
import sys

import overrides, utils
from utils import error, memoize, try_run_command


@memoize
def get(flag='host'):
    if flag == 'host':
        platform_val = overrides.get('CHPL_HOST_PLATFORM')
    elif flag == 'target':
        platform_val = overrides.get('CHPL_TARGET_PLATFORM')
        if not platform_val:
            platform_val = get('host')
    else:
        error("Invalid flag: '{0}'".format(flag), ValueError)

    if not platform_val:
        import chpl_comm
        network = chpl_comm.get_network()
        if network == 'aries':
            platform_val = 'cray-xc'
        elif network == 'ibv':
            # it could be cray-cs, hpe-apollo, or hpe-cray-xd
            # do nothing for now
            pass
        elif network == 'cxi':
            # it could also be hpe-cray-xd, but we can't tell
            # this is good enough, EX is close enough for auto-detection
            platform_val = 'hpe-cray-ex'

    if not platform_val:
        # uname() -> (system, node, release, version, machine, processor)
        uname = platform.uname()
        platform_val = uname[0].lower().replace('_', '')
        machine = uname[4]
        if platform_val == 'linux':
            if 'ppc' in machine:
                endianness = 'le' if 'le' in machine else ''
                bits = '64' if '64' in machine else '32'
                platform_val = 'linux_ppc_{0}{1}'.format(endianness, bits)
            elif machine == 'x86_64':
                build_64_as_32 = os.environ.get('CHPL_BUILD_X86_64_AS_32')
                if build_64_as_32 == "1":
                    platform_val = "linux64_32"
                else:
                    platform_val = "linux64"
            elif machine == 'aarch64' or machine == 'arm64':
                platform_val = "linux64"
            else:
                platform_val = "linux32"
        elif platform_val.startswith("cygwin"):
            if machine == 'x86_64':
                platform_val = "cygwin64"
            else:
                platform_val = "cygwin32"
        elif platform_val.startswith('netbsd'):
            if machine == 'amd64':
                platform_val = 'netbsd64'
            else:
                platform_val = 'netbsd32'

    return platform_val

@memoize
def _get_linux_distribution():
    distribution = 'unknown'

    uname = platform.uname()
    platform_val = uname[0].lower().replace('_', '')
    if platform_val != 'linux':
        return distribution

    os_release = dict()
    try:
        # freedesktop_os_release was added in Python 3.10
        if hasattr(platform, 'freedesktop_os_release'):
            os_release = platform.freedesktop_os_release()
        elif os.path.exists('/etc/os-release'):
            # Fallback for older Python versions
            with open('/etc/os-release') as f:
                for line in f:
                    if '=' in line:
                        key, value = line.strip().split('=', 1)
                        os_release[key] = value.strip('"')
    except OSError:
        pass

    distribution = os_release.get('ID', distribution).lower()
    return distribution

@memoize
def is_fedora():
    distribution = _get_linux_distribution()
    return distribution.startswith('fedora')

@memoize
def is_wsl():
    name = (platform.uname().release).lower()
    if name.endswith('-microsoft') or name.endswith('-microsoft-standard-wsl2'):
        return True

@memoize
def is_hpe_cray(flag='host'):
    platform = get(flag)
    return platform in ('hpe-cray-ex', 'hpe-cray-xd')

@memoize
def is_cray(flag='host'):
    platform = get(flag)
    return platform.startswith('cray') or is_hpe_cray(flag)

@memoize
def is_hpe_apollo(flag='host'):
    platform = get(flag)
    return platform == 'hpe-apollo'

@memoize
def is_cluster(flag='host'):
    return is_cray(flag) or is_hpe_apollo(flag)

@memoize
def get_mac_os_version():
    release, version, machine = platform.mac_ver()
    return release

@memoize
def is_arch_linux():
    arch_file = "/etc/arch-release"
    return os.path.exists(arch_file)

def _main():
    parser = optparse.OptionParser(usage='usage: %prog [--host|target])')
    parser.add_option('--host', dest='flag', action='store_const',
                      const='host', default='host')
    parser.add_option('--target', dest='flag', action='store_const',
                      const='target')
    (options, args) = parser.parse_args()

    platform_val = get(options.flag)
    sys.stdout.write("{0}\n".format(platform_val))


if __name__ == '__main__':
    _main()
