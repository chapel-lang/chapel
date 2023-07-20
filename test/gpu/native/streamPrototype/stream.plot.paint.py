#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import loadDat, joinDat, paint

def process(p):
  p.set_xticks(p.x_data[5:])
  p.set_xticklabels([str(int(l)) for l in p.x_data[5:]])

data = loadDat("logs")
nvidiaDat = joinDat([data['cuda_baseline'], data['nvidia'], data['nvidia_aod']])
amdDat    = joinDat([data['hip_baseline'], data['amd'], data['amd_aod']])

paint("nvidiaDat", nvidiaDat)
paint("amdDat", amdDat)


#d.append(join( data['nvidia_base'], data['nvidia'], data['nvidia_aod'])

#for d in [d1,d2,d3,d4]:
#  d.title  = "Stream ($CHPL_GPU, $CHPL_GPU_MEM_STRATEGY)"
#  d.xlabel = "Number of Elements (M)"
#  d.ylabel = "Throughput\n(GiB/s)"
#  d.better = 'up'
#  d.line[0].style = chplStyle(1)
#  d.paint(process)
