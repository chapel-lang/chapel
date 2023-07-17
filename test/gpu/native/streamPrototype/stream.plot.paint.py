#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import paintDatFiles

def streamSpecificProcessing(p, xData, yData, kvPairs):
  p.set_xticks(xData[5:])
  p.set_xticklabels([str(int(l)) for l in xData[5:]])

paintDatFiles(streamSpecificProcessing)

#data = loadDatFiles()
#paint(data, postProcessFcn=streamSpecificProcessing)
#paintJoined([data['amd.um'], data['amd.aod']],
#            kvpairs={'title': 'AMD UM vs Aod'},
#            postProcessFcnstreamSpecificProcessing);
