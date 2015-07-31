#!/usr/bin/env python
import platform, os, os.path, re, sys, optparse

from utils import memoize

@memoize
def get(flag='host'):
    if flag == 'host':
        platform_val = os.environ.get('CHPL_HOST_PLATFORM')
    elif flag == 'target':
        platform_val = os.environ.get('CHPL_TARGET_PLATFORM')
        if not platform_val:
            platform_val = get('host')
    else:
        raise ValueError("Invalid flag: '{0}'".format(flag))

    if not platform_val:
        # Check for cray platform. It is a cray platform if there is an CLEinfo
        # config file and it has a known network value in it.
        cle_info_file = os.path.abspath('/etc/opt/cray/release/CLEinfo')
        if os.path.exists(cle_info_file):
            with open(cle_info_file, 'r') as fp:
                cle_info = fp.read()
            net_pattern = re.compile('^NETWORK=(?P<net>[a-zA-Z]+)$', re.MULTILINE)
            net_match = net_pattern.search(cle_info)
            if net_match is not None and len(net_match.groups()) == 1:
                net = net_match.group('net')
                if net.lower() == 'gem':
                    platform_val = 'cray-xe'
                elif net.lower() == 'ari':
                    platform_val = 'cray-xc'

    if not platform_val:
        # uname() -> (system, node, release, version, machine, processor)
        uname = platform.uname()
        platform_val = uname[0].lower().replace('_', '')
        machine = uname[4]
        if platform_val == 'linux':
            if machine == 'x86_64':
                build_64_as_32 = os.environ.get('CHPL_BUILD_X86_64_AS_32')
                if build_64_as_32 == "1":
                    platform_val = "linux64_32"
                else:
                    platform_val = "linux64"
            elif machine == 'ppc64':
                platform_val = "linux_ppc64"
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
