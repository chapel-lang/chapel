#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

platform = { 'nvidia': 'cuda',
             'amd': 'hip' }

tbls = load_tables()
for arch in ['nvidia', 'amd']:
  if not arch in tbls:
    continue

  experiment = arch
  experimentBase = arch + "_baseline"
  expPlatform = platform[arch]

  # Generate tables
  jointTbl = join(tbls[experimentBase], tbls[experiment]).with_title(
    "%s vs. chpl execution time (%s)" % (expPlatform, arch))
  normalizedTbl = jointTbl.normalize_to(experimentBase).with_title(
    "chpl execution time over %s time (%s)" % (expPlatform, arch))

  # Print tables
  print()
  print(jointTbl.md())
  print()
  print(normalizedTbl.md())

  # Make plots
  plotArgs = { 'kind': PlotKind.BAR, 'xlabel': 'size' }
  plot(jointTbl,
       filename="logs/chop_%s.png" % arch,
       ylabel="time (s)",
       **plotArgs)
  
  p = plot(normalizedTbl, ylabel="ratio", **plotArgs, 
           save=False)
  p.set_ylim(1.0, 1.3)
  p.save("logs/chop_%s_normalized.png" % arch)
