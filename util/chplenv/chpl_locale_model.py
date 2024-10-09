#!/usr/bin/env python3
import sys

import overrides
from utils import memoize, error


@memoize
def get():
    locale_model_val = overrides.get('CHPL_LOCALE_MODEL', 'flat')

    if locale_model_val not in ['flat', 'gpu']:
        error('{} is not a valid value for CHPL_LOCALE_MODEL. '
              'It can only be "flat" or "gpu".'.format(locale_model_val))

    return locale_model_val


def _main():
    locale_model_val = get()
    sys.stdout.write("{0}\n".format(locale_model_val))


if __name__ == '__main__':
    _main()
