#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
jointTbl = tbls.join()
normalizedTbl = jointTbl.normalize_to('cuda_only')

plotArgs = { 'kind': PlotKind.BAR, 'xlabel': 'size' }

plot(jointTbl,
     title="cuda vs. chpl execution time",
     filename="logs/chop_cuda_vs_chpl_perf.png",
     ylabel="time (s)",
     **plotArgs)

p = plot(jointTbl.normalize_to('cuda_only'),
         title="chpl execution time over cuda time",
         ylabel="ratio",
         **plotArgs,
         save=False)
p.set_ylim(1.0, 1.3)
p.save("logs/normalized.png")
