#!/usr/bin/env python3
import sys

import chpl_locale_model, chpl_tasks, overrides, third_party_utils
from utils import error, memoize, warning, try_run_command


@memoize
def get():
    hwloc_val = overrides.get('CHPL_HWLOC')
    if not hwloc_val:
        tasks_val = chpl_tasks.get()
        if tasks_val == 'qthreads':
            hwloc_val = 'bundled'
        else:
            hwloc_val = 'none'

    return hwloc_val


@memoize
def get_uniq_cfg_path():
    return '{0}-{1}'.format(third_party_utils.default_uniq_cfg_path(),
                            chpl_locale_model.get())


# returns 2-tuple of lists
#  (compiler_bundled_args, compiler_system_args)
@memoize
def get_compile_args():
    hwloc_val = get()
    if hwloc_val == 'bundled':
         ucp_val = get_uniq_cfg_path()
         return third_party_utils.get_bundled_compile_args('hwloc', ucp=ucp_val)

    return ([ ], [ ])


# returns 2-tuple of lists
#  (linker_bundled_args, linker_system_args)
@memoize
def get_link_args():
    hwloc_val = get()
    if hwloc_val == 'bundled':
        return third_party_utils.pkgconfig_get_bundled_link_args(
                                          'hwloc', ucp=get_uniq_cfg_path())
    elif hwloc_val == 'system':
        # Check that hwloc version is OK
        exists, retcode, my_out, my_err = try_run_command(
                            ['pkg-config', '--atleast-version=2.1', 'hwloc'])
        if exists and retcode != 0:
          err = "CHPL_HWLOC=system requires hwloc >= 2.1"
          error(err, ValueError)

        return ([ ], ['-lhwloc'])

    return ([ ], [ ])


def _main():
    hwloc_val = get()
    sys.stdout.write("{0}\n".format(hwloc_val))


if __name__ == '__main__':
    _main()
