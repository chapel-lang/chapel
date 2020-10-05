#!/usr/bin/env python3

import optparse
import os
import re
import sys

"""
Normalizes path components to make them suitable for installation.
For example, since $CHPL_HOME/lib will be installed to CHPL_RUNTIME_LIB,
we replace references to that absolute path with $CHPL_RUNTIME_LIB.
"""

if __name__ == '__main__':

  parser = optparse.OptionParser(usage='usage: %prog [--fixpath key path]')
  parser.add_option('--fixpath', nargs=2, dest='fixpath', action='append');
  
  (options, args) = parser.parse_args()

  if options.fixpath:

    tofix=[]
    for kv in options.fixpath:

      key = kv[0]
      val = kv[1];

      val = os.path.realpath(val)
      tofix.append([key, val])

    #for kv in tofix:
    #  key = kv[0]
    #  val = kv[1];
    #  sys.stdout.write("will replace {0} with {1}\n".format(val,key))

    for line in sys.stdin.readlines():

      # Find things that look like absolute paths
      # Note that this needs to handle e.g. -I/some/directory
      # (and not think it's a relative path starting with I)

      pattern = r'/[^ ]+'

      fixed = line

      for m in re.findall(pattern, line):

        origpath = m
        path = os.path.realpath(origpath)

        if os.path.isfile(path):
          # Leave out the filename, since we're only trying to replace
          # directories. This prevents clang++ from being replaced
          # with e.g. clang-6.0 when clang++ is a symbolic link.
          origpath = os.path.dirname(origpath)
          path = os.path.dirname(path)

        #sys.stdout.write("maybe found path {0} \n".format(path))

        for kv in tofix:

          key = kv[0]
          val = kv[1];

          #sys.stdout.write("does it start with {0}\n".format(val))
          if path.startswith(val):
            rel = os.path.relpath(path, val)
            #sys.stdout.write("rel path to {0} is {1}\n".format(key, rel))
            fixed = fixed.replace(origpath, key + "/" + rel)
            break

          #sys.stdout.write("replacing {0} {1}\n".format(val,key))
          #sys.stdout.write(fixed)

      sys.stdout.write(fixed)

