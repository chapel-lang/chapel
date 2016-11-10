import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_platform
import third_party_utils
from utils import memoize, run_command


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_libunwind_config_file():
    install_path = third_party_utils.get_cfg_install_path('libunwind')
    config_file = os.path.join(install_path, 'lib', 'pkgconfig', 'libunwind.pc')
    return config_file

@memoize
def get_libunwind_la():
    install_path = third_party_utils.get_cfg_install_path('libunwind')
    la = os.path.join(install_path, 'lib', 'libunwind.la')
    return la


@memoize
def get_link_args(unwind):
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    if osx:
      return []

    libs = ['-lunwind']
    if unwind == 'libunwind':
      # the pkg-config file for libunwind is nice, but as of 1.1
      # it is wrong. So try to get the libraries out of libunwind.la
      # if it exists.
      unwind_config = get_libunwind_config_file()
      if os.access(unwind_config, os.R_OK):
        unwind_link = run_command(['pkg-config', '--libs',
                                   unwind_config, '--libs'])
        libs = unwind_link.split()

      unwind_la = get_libunwind_la()
      if os.access(unwind_la, os.R_OK):
        value = ''
        with open(unwind_la) as f:
          for line in f:
            if line.startswith('dependency_libs='):
              value = line.split('=')[1]
              value = value.split("'")[1]
              libs += value.split();

    return libs
