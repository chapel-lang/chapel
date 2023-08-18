#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import loadDat, joinDat, paint

#def process(p):
#  p.set_xticks(p.x_data[5:])
#  p.set_xticklabels([str(int(l)) for l in p.x_data[5:]])

data = loadDat("logs")

if 'cuda_baseline' in data:
  nvidiaDat = joinDat([data['cuda_baseline'], data['nvidia'], data['nvidia_aod']])
  paint("nvidiaDat", nvidiaDat)

if 'hip_baseline' in data:
  amdDat    = joinDat([data['hip_baseline'], data['amd'], data['amd_aod']])
  paint("amdDat", amdDat)
