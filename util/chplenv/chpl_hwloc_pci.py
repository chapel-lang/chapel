#!/usr/bin/env python3

# Implements the CHPL_HWLOC_PCI variable, which controls whether or not the
# bundled hwloc is compiled with PCI discovery enabled. PCI discovery is
# necessary for partitioning PCI devices such as NICs and GPUs among
# co-locales and is therefore enabled when CHPL_COMM != none, or when
# CHPL_LOCALE_MODEL == gpu and CHPL_GPU != cpu. Otherwise it's disabled
# because it introduces an unnecessary dependency on the pciaccess library,
# and PCI discovery has the potential to slow down startup time.

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
