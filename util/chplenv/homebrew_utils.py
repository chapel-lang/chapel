#!/usr/bin/env python3

import sys

from utils import error, memoize, try_run_command

@memoize
def get_homebrew_prefix(pkg=None):
    """
    If running on a system with Homebrew, return the Homebrew prefix.
    If not, return None.

    If pkg is provided, return the Homebrew prefix for that package.
    """
    cmd = ['brew', '--prefix']
    if pkg is not None:
        cmd.append(str(pkg))
    exists, retcode, my_out, _ = try_run_command(cmd)
    if exists and retcode == 0:
        # Make sure to include homebrew search path
        homebrew_prefix = my_out.strip()
        return homebrew_prefix

    return None

@memoize
def homebrew_exists():
    """Check if Homebrew is installed on the system."""
    return get_homebrew_prefix() is not None

@memoize
def homebrew_pkg_exists(pkg):
    """Check if a Homebrew package is installed on the system."""
    cmd = ['brew', 'list', pkg]
    exists, retcode, _, _ = try_run_command(cmd)
    return exists and retcode == 0

def _main():
    sys.stdout.write("{0}\n".format(get_homebrew_prefix()))


if __name__ == '__main__':
    _main()
