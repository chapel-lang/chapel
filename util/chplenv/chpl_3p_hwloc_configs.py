import os
import sys

chplenv_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_locale_model, third_party_utils
from utils import error, memoize


@memoize
def get_uniq_cfg_path():
    return '{0}-{1}'.format(third_party_utils.default_uniq_cfg_path(),
                            chpl_locale_model.get())

@memoize
def get_link_args(hwloc):
    if hwloc == 'hwloc':
        return third_party_utils.default_get_link_args('hwloc',
                                                       ucp=get_uniq_cfg_path())
    elif hwloc == 'system':
        # Check that hwloc version is OK
        okversions = ('1.11.5', '1.11.6', '1.11.7')
        vers = third_party_utils.pkgconfig_get_system_version('hwloc')
        if not vers in okversions:
          err = "CHPL_HWLOC=system but unsupported version {0} was found.\nPlease use one of the following versions {1}\n".format(vers, ' '.join(okversions))
          error(err, ValueError)

        return ['-lhwloc']
    else:
        return []
