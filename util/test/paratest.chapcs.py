#!/usr/bin/env python

"""
Simple slurm-aware paratest wrapper for testing on the chapcs cluster. This
script aims to run parallel testing as quickly as possible without interfering
with nightly testing or other exclusive reservations (e.g.  developers running
performance experiments.) It also tries to interfere as little as possible with
other paratests in order to avoid timeouts.
"""

import os.path
import sys
import timeit

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_comm
import utils


def run_command_wrapper(command):
    """
    Run a command, returning the output as a list of strings with quotes and
    whitespace stripped out.
    """
    output = utils.run_command(command)
    output_lines = [line.strip().strip('"') for line in output.splitlines()]
    return output_lines


def get_num_exclusive_nodes():
    """
    Get the number of nodes reserved exclusively on the chapel partition
    """
    # Grab the "SHARED,NODES" info for all jobs. For each exclusive (SHARED=no)
    # job add up the number of nodes it's using
    squeue_cmd = ['squeue',
                  '--partition=chapel',
                  '--noheader',
                  '--format="%h,%D"']
    squeue_out = run_command_wrapper(squeue_cmd)
    num_exclusive_nodes = 0
    for line in squeue_out:
        shared, num_nodes = line.split(',')
        if shared == 'no':
            num_exclusive_nodes += int(num_nodes)

    return num_exclusive_nodes


def get_num_non_exclusive_nodes():
    """
    Get the number of nodes available for testing on the chapel partition
    (total - exclusive)
    """
    sinfo_cmd = ['sinfo',
                 '--partition=chapel',
                 '--noheader',
                 '--responding',
                 '--format="%D"']
    sinfo_out = run_command_wrapper(sinfo_cmd)
    num_online_nodes = int(sinfo_out[0])
    num_non_exclusive_nodes = num_online_nodes - get_num_exclusive_nodes()
    return num_non_exclusive_nodes


def get_num_shared_jobs_running():
    """
    Get the number of shared jobs running on the chapel partition
    """
    squeue_cmd = ['squeue',
                  '--partition=chapel',
                  '--noheader',
                  '--format="%h"']
    squeue_out = run_command_wrapper(squeue_cmd)
    return squeue_out.count('yes')


def get_good_nodepara():
    """
    Get a "good" nodepara value: use up to 3 for comm=none testing, but limit
    to 1 for comm!=none since that's already oversubscribed.

    TODO: Consider increasing nodepara if no other shared jobs are running.
    Note: need to wait to do this until everybody's using slurm on chapcs.
    """

    nodepara = 3
    if chpl_comm.get() != 'none':
        nodepara = 1
#    if get_num_shared_jobs_running() == 0:
#        nodepara += 1
    return nodepara


def run_paratest(args):
    """
    Run paratest inside an salloc using all nodes that are not reserved
    exclusively on the chapel partition. Throw `--share --nice` and turn off
    affinity and limit how many executables can run at once so we play nice
    with other testing going on.
    """
    nodepara = get_good_nodepara()
    num_free_nodes = get_num_non_exclusive_nodes()
    para_env = ['-env', 'CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes', '-env',
                'QT_AFFINITY=no', '-env', 'QT_SPINCOUNT=300']

    salloc_cmd = ['salloc',
                  '--nodes={0}'.format(num_free_nodes),
                  '--immediate',
                  '--partition=chapel',
                  '--share',
                  '--nice']

    paratest_path = os.path.join(os.path.dirname(__file__), 'paratest.server')

    paratest_cmd = salloc_cmd + [paratest_path] + para_env
    paratest_cmd += ['-nodepara', str(nodepara)] + args
    print('running "{0}"'.format(' '.join(paratest_cmd)))


    start_time = timeit.default_timer()
    for line in utils.run_live_command(paratest_cmd):
        sys.stdout.write(line)
        sys.stdout.flush()
    elapsed = int(timeit.default_timer() - start_time)
    minutes, seconds = divmod(elapsed, 60)
    print('paratest took {0} minutes and {1} seconds'.format(minutes, seconds))


def main(paratest_args):
    run_paratest(paratest_args)


if __name__ == '__main__':
    main(sys.argv[1:])
