import chpl_platform
import third_party_utils
from utils import memoize


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args(unwind):
    platform_val = chpl_platform.get('target')
    osx = platform_val.startswith('darwin')

    # Mac OS X supports libunwind in the C library
    # it's not actually a special library.
    if osx:
      return []

    libs = []
    # Get the link arguments (e.g. -lunwind)
    if unwind == 'system':
      # Try using pkg-config to get the libraries to link
      # libunwind with.
      libs = third_party_utils.pkgconfig_get_link_args(
                       'libunwind', system=True, static=True)
    elif unwind == 'libunwind':
      # the pkg-config file for libunwind is nice, but as of 1.1
      # it doesn't include -lzma when it probably should.
      # So try to get the libraries out of libunwind.la.
      libs = third_party_utils.default_get_link_args(
                       'libunwind', libs=['libunwind.la', 'libunwind-x86_64.la'])

    return libs
