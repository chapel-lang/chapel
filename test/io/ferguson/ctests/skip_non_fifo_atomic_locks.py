#!/usr/bin/env python3

"""Skip test when atomics are implemented with locks and tasking layer is not fifo.

This test requires locks, but the appropriate header files are not available in
non-fifo tasking layers due to how compile line is constructed. Skip the test
for now.
"""

import os
print(os.getenv('CHPL_ATOMICS') == 'locks' and
      os.getenv('CHPL_TASKS') != 'fifo')
