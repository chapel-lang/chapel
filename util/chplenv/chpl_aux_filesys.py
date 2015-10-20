#!/usr/bin/env python
import os
from glob import glob
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

    elif aux_fs == 'hdfs3':
        def fetchInfo(env, val, err):
            stripstr = (env.find('INCLUDE') > 0 and '-I' or '-L')
            files = map(lambda z: z.lstrip(stripstr), os.environ.get(env, '').split())
            res = 0
            for f in files:
               res += sum(map(lambda x: 1, filter(lambda z: z.find(val) > 0 and 1 or 0, glob("%s/*" % (f,)))))

            if res < 1:
                stderr.write(err)
                return False

            return True

        fetchInfo('CHPL_AUXIO_INCLUDE', 'hdfs', "Warning: Can't find your HDFS3 header file installation\n")
        fetchInfo('CHPL_AUXIO_LIBS', 'hdfs3', "Warning: Can't find your HDFS3 static library installation\n")

    return aux_fs


def _main():
    aux_fs_val = get()
    stdout.write("{0}\n".format(aux_fs_val))


if __name__ == '__main__':
    _main()
