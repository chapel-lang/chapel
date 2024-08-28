#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
tbl = tbls.join().with_first_col('mpi')
print(tbl.md("%0.2f"))

plt = tbl.plot(
    title="STREAM Performance (GB/s)",
    xlabel="Nodes",
    ylabel="GB/s",
    save=False)

plt.add_arrow('right', 'up', 'green', 'better')
plt.set_ylim(top=16000)
plt.save('logs/stream.png')
