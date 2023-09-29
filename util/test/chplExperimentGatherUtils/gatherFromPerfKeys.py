#!/usr/bin/env python3

import sys, argparse, re, os

# I've shamelessly copied and adapted this from computePerfStats.py.  Should we
# put this under some common utility module?  The computePerfStats version has
# some additional functionality that deals with specifying the name of a .dat
# file
def read_key_file(keys_file,):
  # read keys from .perfkeys (or other) file
  verify_keys = []
  keys = []
  with open(keys_file, "r") as file:
      for line in file:
          key = line.strip()
          if not key[0] == "#": # not a comment
              st_key = key.strip()[0:6]
              if "verify" == st_key or "reject" == st_key:
                  verify_keys.append(key)
              else:
                  keys.append(key)

  return (keys, verify_keys)


def find_keys(keys, file):
  results = {}

  keyRgxps = {}
  for key in keys:
    keyRgxps[key] = re.compile(re.escape(key) + r"\s*(\S*)")
    results[key] = []

  for line in file:
    line = line.strip()
    for key in keys:
      m = keyRgxps[key].search(line)
      if m:
        results[key].append(m.group(1))

  return results

parser = argparse.ArgumentParser("Prequal launcher")
parser.add_argument('--fromDir')
parser.add_argument('--runlog', required=True)
parser.add_argument('--keyfile', required=True)
parser.add_argument('--rows', nargs='*')
parser.add_argument('--columns', nargs="*")

args = parser.parse_args(sys.argv[1:])

if args.fromDir is not None:
  os.chdir(args.fromDir)

(keys, verify_keys) = read_key_file(args.keyfile)
res = find_keys(keys, open(args.runlog))

for col in args.columns:
  print("\t" + col, end="")
print()
for (count, row) in enumerate(args.rows):
  print(row, end="")
  for key in keys:
    print("\t", res[key][count], end="")
  print()
