#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()
print(tbls.md())
tbls.plot(
    xlabel="Matrix size (1000 N x 1000 N)",
    ylabel="Throughput\n(GiB/s)")
