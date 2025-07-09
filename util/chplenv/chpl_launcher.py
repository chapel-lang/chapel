#!/usr/bin/env python3

import sys

import chpl_comm, chpl_comm_substrate, chpl_platform, overrides
from utils import which, error, memoize, warning, check_valid_var


def slurm_prefix(base_launcher):
    """ If srun|salloc is available, prefix with slurm-"""
    if which('srun') or which('salloc'):
        return 'slurm-{}'.format(base_launcher)
    return base_launcher

@memoize
def get():
    launcher_val = overrides.get('CHPL_LAUNCHER')

    comm_val = chpl_comm.get()
    substrate_val = chpl_comm_substrate.get()
    if comm_val == 'gasnet' and substrate_val == 'udp':
        if not launcher_val:
            launcher_val = 'amudprun'
        elif launcher_val not in ('none', 'amudprun'):
            error('CHPL_LAUNCHER={} is not supported for CHPL_COMM=gasnet '
                  'CHPL_COMM_SUBSTRATE=udp, CHPL_LAUNCHER=amudprun is '
                  'required'.format(launcher_val))

    if not launcher_val:
        if comm_val == 'gasnet':
            if substrate_val == 'smp':
                launcher_val = 'smp'
            elif substrate_val == 'mpi':
                launcher_val = slurm_prefix('gasnetrun_mpi')
            elif substrate_val == 'ibv':
                launcher_val = slurm_prefix('gasnetrun_ibv')
            elif substrate_val == 'ucx':
                launcher_val = slurm_prefix('gasnetrun_ucx')
            elif substrate_val == 'ofi':
                launcher_val = slurm_prefix('gasnetrun_ofi')
        else:
            if chpl_platform.is_cluster('target'):
                has_aprun = which('aprun')
                has_slurm = which('srun')
                if has_aprun and has_slurm:
                    launcher_val = 'none'
                    warning("Both 'aprun' and 'srun' are available on this system. Please explicitly set CHPL_LAUNCHER.")
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
                    launcher_val = 'none'
                    warning('Cannot detect launcher on this system. Please '
                            'set CHPL_LAUNCHER in the environment.')
            else:
                if comm_val != "none" and which('srun'):
                    launcher_val = 'slurm-srun'
                else:
                    launcher_val = 'none'

    if launcher_val is None:
        launcher_val = 'none'

    gasnet_launchers = ["mpi", "ibv", "ucx", "ofi"]
    valid_values = ["none", "amudprun", "smp", "aprun", "slurm-srun"]
    valid_values.extend(["lsf-gasnetrun_ibv", "mpirun", "mpirun4ofi", "pals", "pbs-aprun", "pbs-gasnetrun_ibv"])
    valid_values.extend(["gasnetrun_{}".format(l) for l in gasnet_launchers])
    valid_values.extend(["slurm-gasnetrun_{}".format(l) for l in gasnet_launchers])
    check_valid_var("CHPL_LAUNCHER", launcher_val, valid_values)

    return launcher_val


def _main():
    launcher_val = get()
    sys.stdout.write("{0}\n".format(launcher_val))


if __name__ == '__main__':
    _main()
