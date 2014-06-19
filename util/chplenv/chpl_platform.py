#!/usr/bin/env python
import platform, os, sys, optparse

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
        # uname() -> (system, node, release, version, machine, processor)
        uname = platform.uname()
        platform_val = uname[0].lower().replace('_', '')
        if platform_val == 'linux':
            machine = uname[4]
            if machine == 'x86_64':
                build_64_as_32 = os.environ.get('CHPL_BUILD_X86_64_AS_32')
                if build_64_as_32 == "1":
                    platform_val = "linux64_32"
                else:
                    platform_val = "linux64"
            else:
                platform_val = "linux32"
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
