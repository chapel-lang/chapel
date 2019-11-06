import chpl_locale_model, third_party_utils
from utils import error, memoize


@memoize
def get_uniq_cfg_path():
    return third_party_utils.default_uniq_cfg_path()

@memoize
def get_link_args(libfabric):
    libs = []
    if libfabric == 'system':
      # Try using pkg-config to get the libraries to link
      # libfabric with.
      pclibs = third_party_utils.pkgconfig_get_link_args(
                       'libfabric', system=True)
      for pcl in pclibs:
        libs.append(pcl)
        if pcl.startswith('-L'):
          libs.append(pcl.replace('-L', '-Wl,-rpath,', 1))
    elif libfabric == 'libfabric':
      error("CHPL_LIBFABRIC=libfabric is not yet supported", ValueError)

    return libs
