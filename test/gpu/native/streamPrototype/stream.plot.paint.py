#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import paintDatFiles

def process(p):
  p.set_xticks(p.x_data[5:])
  p.set_xticklabels([str(int(l)) for l in p.x_data[5:]])

  if(p.baseName = 'result.amd.dat'):
    p.set_title('Stream (using AMD Instinct MI100)')

paintDatFiles(process)
