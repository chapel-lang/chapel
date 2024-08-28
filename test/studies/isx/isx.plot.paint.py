#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
tbl = tbls.join()
if 'shmem' in tbls:
  tbl = tbl.with_first_col('shmem')
print(tbl.md("%0.2f"))

tbl.plot(
    title="ISx Time",
    xlabel="Nodes",
    ylabel="Time (sec)",
    save=False).add_arrow('right', 'down', 'green', 'faster').save()
