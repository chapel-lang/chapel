#!/usr/bin/env python
import os
from sys import stdout, stderr

import chpl_tasks
from utils import memoize

@memoize
def get():
    if 'CHPL_THREADS' in os.environ:
        stderr.write('Warning: Explicit CHPL_THREADS value ("{0}") is ignored.\n'.format(
                     os.environ['CHPL_THREADS']))
    tasks_val = chpl_tasks.get()
    if tasks_val == 'fifo':
        threads_val = 'pthreads'
    elif tasks_val == 'massivethreads':
        threads_val = 'none'
    elif tasks_val == 'muxed':
        threads_val = 'soft-threads'
    elif tasks_val == 'qthreads':
        threads_val = 'none'
    else:
        stderr.write('Warning: Unexpected tasks value {0}\n'.format(tasks_val))
        threads_val = ''
    return threads_val


def _main():
    threads_val = get()
    stdout.write("{0}\n".format(threads_val))


if __name__ == '__main__':
    _main()
