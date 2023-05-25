#!/usr/bin/env python3

def sysLauncherName(chplLauncher):
    sysLaunchers = dict([('amudprun',             'amudprun'),
                         ('aprun',                'aprun'),
                         ('dummy',                ''),
                         ('gasnetrun_ibv',        'gasnetrun_ibv'),
                         ('mpirun4ofi',           'mpirun'),
                         ('slurm-gasnetrun_ibv',  'gasnetrun_ibv'),
                         ('slurm-srun',           'srun'),
                         ('smp',                  ''),
                        ])
    if chplLauncher in sysLaunchers:
        return sysLaunchers[chplLauncher]
    return None
