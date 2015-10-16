#!/usr/bin/env python
import os
import sys

from utils import memoize


@memoize
def get():
    locale_model_val = os.environ.get('CHPL_LOCALE_MODEL', 'flat')
    return locale_model_val


def _main():
    locale_model_val = get()
    sys.stdout.write("{0}\n".format(locale_model_val))


if __name__ == '__main__':
    _main()
