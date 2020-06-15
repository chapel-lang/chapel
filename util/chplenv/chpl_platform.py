#!/usr/bin/env python
import optparse
import os
import platform
import re
import sys

import overrides, utils
from utils import error, memoize


@memoize
def get(flag='host'):
    if flag == 'host':
        platform_val = overrides.get('CHPL_HOST_PLATFORM')
    elif flag == 'target':
        platform_val = overrides.get('CHPL_TARGET_PLATFORM')
        if not platform_val:
            platform_val = get('host')
    else:
        raise error("Invalid flag: '{0}'".format(flag), ValueError)

    if not platform_val:
        # Check for cray platform. It is a cray platform if there is a
        # cle-release/CLEinfo config file with a known network value in it.
        cle_info_file = os.path.abspath('/etc/opt/cray/release/cle-release') # CLE >= 6
        if not os.path.exists(cle_info_file):
            cle_info_file = os.path.abspath('/etc/opt/cray/release/CLEinfo') # CLE <= 5

        if os.path.exists(cle_info_file):
            with open(cle_info_file, 'r') as fp:
                cle_info = fp.read()
            net_pattern = re.compile('^NETWORK=(?P<net>[a-zA-Z]+)$', re.MULTILINE)
            net_match = net_pattern.search(cle_info)
            if net_match is not None and len(net_match.groups()) == 1:
                net = net_match.group('net')
                if net.lower() == 'ari':
                    platform_val = 'cray-xc'

    if not platform_val:
        network = os.environ.get('CRAYPE_NETWORK_TARGET', '')
        if network.startswith("slingshot") or network == "ofi":
            platform_val = 'cray-shasta'

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
            elif machine == 'aarch64':
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
def is_cross_compiling():
    return get('host') != get('target')


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
