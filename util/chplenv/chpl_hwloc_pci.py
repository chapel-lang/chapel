#!/usr/bin/env python3
import sys

import chpl_hwloc, chpl_comm, chpl_locale_model, chpl_gpu
import overrides

from utils import error, memoize

@memoize
def get():
    hwloc_val = chpl_hwloc.get()
    if hwloc_val != 'bundled':
        return 'disable'

    pci_val = overrides.get('CHPL_HWLOC_PCI')
    if not pci_val:
        pci_val = 'disable'
        comm_val = chpl_comm.get()
        if comm_val != 'none':
            pci_val = 'enable'
        else:
            locale_val = chpl_locale_model.get()
            if locale_val == 'gpu':
                gpu_val = chpl_gpu.get()
                if gpu_val != 'cpu':
                    pci_val = 'enable'
    return pci_val


def _main():
    pci_val = get()
    sys.stdout.write("{0}\n".format(pci_val))


if __name__ == '__main__':
    _main()
