#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import *

tbls = load_tables()

tbls.plot(
    title="STREAM Performance (GB/s)",
    xlabel="Locales",
    ylabel="GB/s")
