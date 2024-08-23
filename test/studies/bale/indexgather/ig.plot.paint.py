#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
print(tbls.join().md("%0.2f"))


tbl_fine = tbls.join().select('shmem.agp', 'chpl_fine_1lpn', 'chpl_fine_2lpn')
tbl_agg  = tbls.join().select('shmem.exstack', 'shmem.conveyor', 'chpl_agg_1lpn', 'chpl_agg_2lpn')
print(tbl_fine.md("%0.2f"))
print()
print(tbl_agg.md("%0.2f"))

plt_fine = tbl_fine.plot(
    title="Bale Indexgather Performance",
    xlabel="Nodes",
    ylabel="GB/s",
    lineStyles={'shmem.agp': 1,
                'chpl_fine_1lpn': 2,
                'chpl_fine_2lpn': 12},
    save=False).add_arrow('right', 'up', 'green', 'better')
plt_fine.mpl_legend_args = { 'fontsize': 'medium' }
plt_fine.save('logs/fine.png')

plt_agg = tbl_agg.plot(
    title="Bale Indexgather Performance",
    xlabel="Nodes",
    ylabel="GB/s",
    lineStyles={'shmem.exstack': 1,
                'shmem.conveyor': 11,
                'chpl_agg_1lpn': 2,
                'chpl_agg_2lpn': 12},
    save=False).add_arrow('right', 'up', 'green', 'better')
plt_agg.mpl_legend_args = { 'fontsize': 'medium' }
plt_agg.save('logs/agg.png')
