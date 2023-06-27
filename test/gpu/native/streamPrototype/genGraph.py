#!/usr/bin/env python3

import sys, os
sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'test'))
from chpl_plot import load, Plot

dataFile = 'log/result.dat' if len(sys.argv) == 0 else sys.argv[1]
(vecsize, data) = load(dataFile)
cudaOrHip = 'cuda' if 'cuda' in data.keys() else 'hip'

vecsize = [int(l) for l in vecsize]

p = Plot(name='GPU Stream', x_data=vecsize)

p.add_arrow(position='right', direction='up', text='Better', color='green')

p.add_y_data(data[cudaOrHip], linestyle=11, label=cudaOrHip)
p.add_y_data(data['chpl'], linestyle=3, label='Chapel')

p.set_title('Stream')
p.set_xlabel('Number of Elements (M)')
p.set_ylabel('Throughput\n(GiB/s)')
p.set_xticks(vecsize[5:])
p.set_xticklabels([str(l) for l in vecsize[5:]])

p.legend_font_size = 18

p.show()
