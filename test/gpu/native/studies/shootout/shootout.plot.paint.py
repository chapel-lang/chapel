#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

def stripPathToFile(xData):
  return list(map(lambda x: x.split('/')[-1], xData))

tbls = load_tables()
tbl_compile_times = tbls.select('compile_time').relabel_col_w_tbl_title().join()
tbl_compile_times.xData = stripPathToFile(tbl_compile_times.xData)

tbl_exec_times = tbls.select('exec_time').relabel_col_w_tbl_title().join()
tbl_exec_times.xData = stripPathToFile(tbl_exec_times.xData)

center_legend = { 'loc': 'center',
                  'ncol': 3,
                  'bbox_to_anchor': (0.5, 1.3) }

p = tbl_compile_times.plot(
  title='Shootout benchmark compile times',
  xlabel='Benchmark',
  ylabel='Time (s)',
  better='down',
  save=False)
p.mpl_legend_args = center_legend
p.save('logs/compile_times.png', bbox_inches='tight', pad_inches=0.5)

p = tbl_exec_times.plot(
  title='Shootout benchmark execution times',
  xlabel='Benchmark',
  ylabel='Time (s)',
  better='down',
  save=False)
p.set_ylim((0,2))
p.mpl_legend_args = center_legend
p.save('logs/execution_times.png')#, bbox_inches='tight', pad_inches=0.5)
