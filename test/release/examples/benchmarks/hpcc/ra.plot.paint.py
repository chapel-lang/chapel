#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
tbl = tbls.join()
if 'mpi' in tbls:
  tbl = tbl.with_first_col('mpi')
print(tbl.md("%0.4f"))

tbl.plot(
    title="RA Performance (GUPS)",
    xlabel="Locales",
    ylabel="GB/s")
