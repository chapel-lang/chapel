#!/usr/bin/env python3

import sys

def lineSourcesPrelude(line):
  if not line.startswith('source ') or not line.endswith("prelude.bash $@"):
    return False
  return True

def processFile(f):
  for line in f:
    strippedLine = line.strip()
    if strippedLine == '# !<IGNORE_CHPL_EXPERIMENT_PRELUDE>!':
      return True
    if strippedLine == '' or strippedLine[0] == '#':
      continue

    return lineSourcesPrelude(strippedLine)
  return False


if len(sys.argv) == 1:
  print(f"{sys.argv[0]} passed an invalid number of arguments")
  print(f"usage: {sys.argv[0]} file", file=sys.stderr)

filename = sys.argv[1]

with open(sys.argv[1]) as f:
  if not processFile(f):
    print(f"Script is missing prelude. Add the following line to the top of {filename}", file=sys.stderr)
    print(f"(it must be the first non-blank, non-commented line):", file=sys.stderr)
    print(f"", file=sys.stderr)
    print(f"    source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@", file=sys.stderr)
    print("")
    print("Or you may supress this error by adding # !<IGNORE_CHPL_EXPERIMENT_PRELUDE>!")
    sys.exit(1)

sys.exit(0)
