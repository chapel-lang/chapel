#!/usr/bin/env python
import os
from sys import stdout, stderr

from utils import memoize

@memoize
def get():
    aux_fs = os.environ.get('CHPL_AUX_FILESYS', 'none')

    if aux_fs == 'hdfs':
        java_subdir = os.environ.get('JAVA_INSTALL', '')
        aux_fs_subdir = os.environ.get('HADOOP_INSTALL', '')

        # This will not check that all dependencies are satisfied..
        found_java = os.path.isdir(os.path.join(java_subdir, 'include'))
        found_hdfs = os.path.exists(os.path.join(aux_fs_subdir, 'src', 'c++',
                                                 'libhdfs', 'hdfs.h'))
        if not found_java:
            stderr.write("Warning: Can't find your Java installation\n")
        if not found_hdfs:
            stderr.write("Warning: Can't find your Hadoop installation\n")
    return aux_fs


def _main():
    aux_fs_val = get()
    stdout.write("{0}\n".format(aux_fs_val))


if __name__ == '__main__':
    _main()
