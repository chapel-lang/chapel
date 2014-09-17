#!/usr/bin/env python
import sys, os

import chpl_comm, chpl_comm_substrate, chpl_platform, chpl_compiler, utils
from utils import memoize

@memoize
def get():
    launcher_val = os.environ.get('CHPL_LAUNCHER')
    if not launcher_val:
        comm_val = chpl_comm.get()
        platform_val = chpl_platform.get('target')
        compiler_val = chpl_compiler.get('target')

        if platform_val.startswith('cray-x'):
            has_aprun = utils.find_executable('aprun')
            has_slurm = utils.find_executable('srun')
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
        elif platform_val == 'marenostrum':
            launcher_val = 'marenostrum'
        elif compiler_val == 'tile-cc':
            launcher_val = 'tile-monitor'
        elif comm_val == 'gasnet':
            substrate_val = chpl_comm_substrate.get()
            if substrate_val == 'udp':
                launcher_val = 'amudprun'
            elif substrate_val == 'mpi':
                launcher_val = 'gasnetrun_mpi'
            elif substrate_val == 'ibv':
                if platform_val == 'pwr6':
                    # our loadleveler launcher is not yet portable/stable/
                    # flexible enough to serve as a good default
                    #launcher_val = 'loadleveler'
                    launcher_val = 'none'
                else:
                    launcher_val = 'gasnetrun_ibv'
            elif substrate_val == 'mxm':
                launcher_val = 'gasnetrun_ibv'
            elif substrate_val == 'lapi':
                # our loadleveler launcher is not yet portable/stable/flexible
                # enough to serve as a good default
                #launcher_val = 'loadleveler'
                launcher_val = 'none'
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
