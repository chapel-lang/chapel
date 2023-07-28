#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
jointTbl = tbls.join()
normalizedTbl = jointTbl.normalize_to('cuda_only')

print(normalizedTbl.md())


plotArgs = { 'kind': PlotKind.BAR, 'xlabel': 'size', 'ylabel': 'time (s)' }
plot(jointTbl, **plotArgs)
p = plot(jointTbl.normalize_to('cuda_only'), **plotArgs, save=False)
p.set_ylim(1.0, 1.3)
p.save()
