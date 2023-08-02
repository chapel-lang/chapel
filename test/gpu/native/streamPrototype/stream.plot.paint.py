#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
p = tbls.plot(save=False)
p.set_xticks(p._x_data[5:])
p.set_xticklabels([str(int(l)) for l in p._x_data[5:]])
p.save('logs/stream.png')

#if 'cuda_baseline' in tbls:
#  nvidiaDat = joinDat([data['cuda_baseline'], data['nvidia'], data['nvidia_aod']])
#  paint("nvidiaDat", nvidiaDat)
#
#if 'hip_baseline' in tbls:
#  amdDat    = joinDat([data['hip_baseline'], data['amd'], data['amd_aod']])
#  paint("amdDat", amdDat)
