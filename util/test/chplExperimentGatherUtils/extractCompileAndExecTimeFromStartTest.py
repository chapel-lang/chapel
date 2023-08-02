#!/usr/bin/env python

import re, sys, os, subprocess, functools

re1 = re.compile(r'Elapsed compilation time for \"([^\s]+)\s*(\(compopts: \d+\))?\" - ([\d\.]+) seconds')
re2 = re.compile(r'Elapsed execution time for \"([^\s]+)\s*(\(execopts: \d+\))?\" - ([\d\.]+) seconds')

compilationTimes = {}
execTimes = {}

if len(sys.argv) != 2:
  print("Invalid number of arguments passed to " +
        "extractCompileAndExecTimeFromStartTest.py")
  sys.exit(1)

logFile = sys.argv[1]
if not os.path.isfile(logFile):
  print("Could not find file: ", logFile);
  sys.exit(1)

with open(logFile) as f:
  for line in f:
    for (res, rgxp) in [(compilationTimes, re1), (execTimes, re2)]:
      if m := rgxp.search(line):
        res[m.group(1)] = m.group(3)

maxTestNameLen = functools.reduce(lambda a,b: max(a, len(b)), compilationTimes.keys(), 0)

print(f"%-{maxTestNameLen}s\tcompile_time\texec_time" % "")
for test in compilationTimes.keys():
  cTime = compilationTimes[test]
  eTime = execTimes[test]
  print(f"%-{maxTestNameLen}s\t{cTime}\t\t{eTime}" % (test))
