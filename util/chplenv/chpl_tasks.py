#!/usr/bin/env python
import sys, os

import chpl_locale_model
from utils import memoize

@memoize
def get():
    tasks_val = os.environ.get('CHPL_TASKS')
    if not tasks_val:
        locale_model_val = chpl_locale_model.get()
        if locale_model_val == 'numa':
            tasks_val = 'qthreads'
        else:
            tasks_val = 'fifo'
    return tasks_val


def _main():
    tasks_val = get()
    sys.stdout.write("{0}\n".format(tasks_val))


if __name__ == '__main__':
    _main()
