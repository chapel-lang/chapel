#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

def stripPathToFile(xData):
  return list(map(lambda x: x.split('/')[-1], xData))

tbls = load_tables()
tbl_compile_times = tbls.select('compile_time').with_col_as_tbl_title().join().with_title('compile time')
tbl_exec_times    = tbls.select('exec_time')   .with_col_as_tbl_title().join().with_title('execution time')

def fixupTables(tbl):
  tbl.xData = stripPathToFile(tbl_compile_times.xData)
  tbl = tbl.with_cols_relabeled({
    '01_flat': 'flat',
    '02_flat_gasnet': 'gasnet',
    '03_gpu_cpu': 'cpu-as-device',
    '04_gpu': 'gpu',
    '05_gpu_w_spec': 'GPU w/ specialization'})
  return tbl

tbl_nrm_compile_times = tbl_compile_times.normalize_to('01_flat').with_title('compile time ratio (over time w/ flat locale model)')
tbl_nrm_exec_times    = tbl_exec_times   .normalize_to('01_flat').with_title('execution time ratio (over time w/ flat locale model)')

[tbl_compile_times, tbl_exec_times, tbl_nrm_compile_times, tbl_nrm_exec_times] = \
  map(fixupTables, [tbl_compile_times, tbl_exec_times, tbl_nrm_compile_times, tbl_nrm_exec_times])

center_legend = { 'loc': 'center', 'ncol': 2, 'bbox_to_anchor': (0.5, 1.5) }
def report(tbl, filename, ylim=None, shiftLineStyles=0):
  print()
  print(tbl.md("%.2f"))
  p = tbl.plot(
    xlabel='Benchmark',
    ylabel='Time (s)',
    better='down',
    shiftLineStyles=shiftLineStyles,
    save=False)

  p.mpl_legend_args = center_legend
  if ylim is not None:
    p.set_ylim(ylim)
  p.save('logs/' + filename, bbox_inches='tight', pad_inches=0.5)

report(tbl_compile_times, '01__compile_time.png', ylim=(0,20))
report(tbl_exec_times,    '02__exec_time.png', ylim=(0,7))
report(tbl_nrm_compile_times, '03__nrm_compile_time.png', ylim=(1,5), shiftLineStyles=1)
report(tbl_nrm_exec_times,    '04__nrm_exec_time.png', ylim=(1,40), shiftLineStyles=1)
