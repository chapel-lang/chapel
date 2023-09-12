#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()

tbl_amd = None
tbl_nvidia = None
if 'amd' in tbls:
  tbl_amd = join(tbls['hip_baseline'], tbls['amd']).with_title(
    'Stream on %s AMD GPU' % os.getenv('CHPL_GPU_ARCH'))
if 'nvidia' in tbls:
  tbl_nvidia = join(tbls['cuda_baseline'], tbls['nvidia']).with_title(
    'Stream on NVIDIA GPU')

def report(tbl, filename):
  if tbl is None:
    return None
  print()
  print(tbl.md("%0.1f"))
  p = tbl.plot(
    xlabel="Number of Elements (M)",
    ylabel="Throughput\n(GiB/s)",
    save=False)
  p.set_xticks(p._x_data[5:])
  p.set_xticklabels([str(int(l)) for l in p._x_data[5:]])
  p.save(f'logs/{filename}')

report(tbl_amd, "stream_amd.png")
report(tbl_nvidia, "nvidia_amd.png")
