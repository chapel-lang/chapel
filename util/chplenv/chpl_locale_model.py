#!/usr/bin/env python3
import sys

import overrides
from utils import memoize, error, check_valid_var


@memoize
def get():
    locale_model_val = overrides.get('CHPL_LOCALE_MODEL', 'flat')
    check_valid_var("CHPL_LOCALE_MODEL", locale_model_val, ["flat", "gpu"])
    return locale_model_val


def _main():
    locale_model_val = get()
    sys.stdout.write("{0}\n".format(locale_model_val))


if __name__ == '__main__':
    _main()
