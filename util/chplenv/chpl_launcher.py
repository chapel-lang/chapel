#!/usr/bin/env python
from distutils.spawn import find_executable
import sys

import chpl_comm, chpl_comm_substrate, chpl_platform, overrides
from utils import memoize


@memoize
def get():
    launcher_val = overrides.get('CHPL_LAUNCHER')
    if not launcher_val:
        comm_val = chpl_comm.get()
        platform_val = chpl_platform.get('target')

        if platform_val.startswith('cray-') or platform_val.startswith('hpe-cray-'):
            has_aprun = find_executable('aprun')
            has_slurm = find_executable('srun')
            if has_aprun and has_slurm:
                launcher_val = 'none'
            elif has_aprun:
                launcher_val = 'aprun'
            elif has_slurm:
                launcher_val = 'slurm-srun'
            else:
                # FIXME: Need to detect aprun/srun differently. On a cray
                #        system with an eslogin node, it is possible that aprun
                #        will not be available on the eslogin node (only on the
                #        login node).
                #
                #        has_aprun and has_slurm should look other places
                #        (maybe the modules?) to decide.
                #        (thomasvandoren, 2014-08-12)
                sys.stderr.write(
                    'Warning: Cannot detect launcher on this system. Please '
                    'set CHPL_LAUNCHER in the environment.\n')
        elif comm_val == 'gasnet':
            substrate_val = chpl_comm_substrate.get()
            if substrate_val == 'smp':
                launcher_val = 'smp'
            elif substrate_val == 'udp':
                launcher_val = 'amudprun'
            elif substrate_val == 'mpi':
                launcher_val = 'gasnetrun_mpi'
            elif substrate_val == 'ibv':
                launcher_val = 'gasnetrun_ibv'
            elif substrate_val == 'mxm':
                launcher_val = 'gasnetrun_ibv'
            elif substrate_val == 'ofi':
                launcher_val = 'gasnetrun_ofi'
            elif substrate_val == 'psm':
                launcher_val = 'gasnetrun_psm'
        elif comm_val == 'mpi':
            launcher_val = 'mpirun'
        else:
            launcher_val = 'none'

    if launcher_val is None:
        launcher_val = 'none'

    return launcher_val


def _main():
    launcher_val = get()
    sys.stdout.write("{0}\n".format(launcher_val))


if __name__ == '__main__':
    _main()
