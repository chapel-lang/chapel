#!/usr/bin/env python3

"""
Locale-per-socket tests. Usage: ./lps_test.py. The -v flag prints
verbose output, the -f flag will cause testing to stop when the
first test fails.
"""

import unittest
import subprocess
import os
import sys
from pprint import pprint

verbose = False

def run(cmd, env=None):
    if type(cmd) is str:
        cmd = cmd.split()
    if env is None:
        proc = subprocess.run(cmd, text=True, check=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    else:
        proc = subprocess.run(cmd, text=True, check=True, env=env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return proc

def checkConfig():
    # These tests only run on comm=ofi and slurm-srun
    proc = run("printchplenv --simple")
    for line in proc.stdout.splitlines():
        (key, value) = line.split('=')
        if key == 'CHPL_COMM' and value != 'ofi':
            raise unittest.SkipTest("CHPL_COMM != ofi")
        if key == 'CHPL_LAUNCHER' and value != 'slurm-srun':
            raise unittest.SkipTest("CHPL_LAUNCHER != slurm-srun")

class LocalePerSocket(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        global verbose
        checkConfig()
        cmd = ["sinfo", "--format=%X %Y %Z", "--noheader", "--exact"]
        if 'CHPL_LAUNCHER_PARTITION' in os.environ:
            cmd += ["--partition", os.environ['CHPL_LAUNCHER_PARTITION']]
            if verbose:
                print("Partition: ", os.environ['CHPL_LAUNCHER_PARTITION'])

        proc = run(cmd)

        (cls.sockets, cls.cores, cls.threads) = \
            [int(i) for i in proc.stdout.splitlines()[0].split()]

        if verbose:
            print("Sockets: ", cls.sockets)
            print("Cores/Socket: ", cls.cores)
            print("Threads/Core: ", cls.threads)

        # We need the qthread output
        os.environ['QTHREAD_INFO'] = '2'

        # Should not be set by default
        if 'CHPL_RT_LOCALES_PER_NODE' in os.environ:
            del os.environ['CHPL_RT_LOCALES_PER_NODE']

        # Compile the test program
        if verbose:
            print("Compiling test program")
        run('chpl lps_test.chpl')
        if verbose:
            print("Running tests")

    def setUp(self):
        pass

    def getCores(self, socket, reserved = 0):
        if socket == "all":
            return ':'.join([self.getCores(i)
                            for i in range(0,self.sockets)])
        else:
            return ':'.join([str(i + (socket * self.cores))
                            for i in range(0,self.cores-reserved)])

    def getThreads(self, socket, reserved = 0):
        if socket == "all":
            return ':'.join([self.getCores(i, reserved)
                        for i in range(0,self.sockets * self.threads)])
        return ':'.join([self.getCores(socket + i * self.sockets,
                        reserved) for i in range(0, self.threads)])

    # Check standard srun arguments
    def checkArgs(self, nodes, tasks, tasksPerNode, cpusPerTask,
                  cpuBind, output):
        self.assertIn('--nodes=%d' % nodes, output)
        self.assertIn('--ntasks=%d' % tasks, output)
        self.assertIn('--cpus-per-task=%d' % cpusPerTask, output)
        if (cpuBind != None):
            self.assertIn('--cpu-bind=%s' % cpuBind, output)
        else:
            self.assertNotIn('--cpu-bind', output)

    def mrAllocatedRequired(self):
        proc = run("fi_info -v")
        for line in proc.stdout.splitlines():
            if "mr_mode" in line:
                if "FI_MR_ALLOCATED" in line:
                    return True
                else:
                    break
        return False

    def test_00_base(self):
        global cls
        proc = run("./lps_test -nl 2 -v")
        self.checkArgs(2, 2, 1, self.sockets * self.cores * self.threads,
                       None, proc.stdout)
        self.assertIn('oversubscribed = False', proc.stdout)
        self.assertIn('Using %d Shepherds' % (self.sockets * self.cores),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"),
                      proc.stdout)

    def test_01_oversubscribed_env(self):
        # Being oversubscribed should have no effect on the number
        # of shepherds.
        if self.mrAllocatedRequired():
            self.skipTest("FI_MR_ALLOCATED is required by provider")
        env = os.environ.copy()
        env['CHPL_RT_OVERSUBSCRIBED'] = 'true'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(2, 2, 1, self.sockets * self.cores * self.threads,
                       None, proc.stdout)
        self.assertIn('oversubscribed = True', proc.stdout)
        self.assertIn('Using %s Shepherds' % (self.sockets * self.cores),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"), proc.stdout)

    def test_02_two_lpn(self):
        # One locale per socket. Each locale only uses the cores in
        # its socket.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %s Shepherds' % self.cores, proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), proc.stdout)

    def test_03_four_lpn(self):
        # Four locales per node is an error if CHPL_RT_LOCALES_PER_NODE
        # is set.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '4'
        proc = run(['./lps_test', '-nl', '4', '-v'], env=env)
        self.assertIn('error: The number of locales on the node is '
                      'greater than the number of sockets (4 > 2).',
                      proc.stdout)

    def test_04_one_locale(self):
        # One locale uses only the cores in its socket.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        proc = run(['./lps_test', '-nl', '1', '-v'], env=env)
        self.checkArgs(1, 1, 2, self.sockets * self.cores * self.threads,
                       'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('Using %s Shepherds' % self.cores, proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), proc.stdout)

    def test_05_two_lpn_oversubscribed(self):
        # CHPL_RT_OVERSUBSCRIBED should have no effect when there is
        # one locale per socket
        if self.mrAllocatedRequired():
            self.skipTest("FI_MR_ALLOCATED is required by provider")
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_OVERSUBSCRIBED'] = 'true'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('oversubscribed = True', proc.stdout)
        self.assertIn('Using %s Shepherds' % self.cores, proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), proc.stdout)

    def test_06_no_ht(self):
        # If SLURM_HINT=nomultithread then the launcher should request
        # only one PU per core and the locales should only use the
        # same.
        env = os.environ.copy()
        env['SLURM_HINT'] = 'nomultithread'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(2, 2, 1, self.sockets * self.cores, None, proc.stdout)
        self.assertNotIn('using socket', proc.stdout)
        self.assertNotIn('using socket', proc.stdout)
        self.assertIn('Using %s Shepherds' % (self.sockets * self.cores),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"), proc.stdout)

    def test_07_two_lpn_no_ht(self):
        # If SLURM_HINT=nomultithread then each locale should only use one PU
        # per core.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['SLURM_HINT'] = 'nomultithread'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %s Shepherds' % self.cores, proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), proc.stdout)

    def test_08_ht_shepherds(self):
        # One worker per PU with hyperthreading.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = \
            '%s' % (self.sockets * self.cores)
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %s Shepherds' % (self.cores * self.threads),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getThreads(0), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getThreads(1), proc.stdout)

    def test_09_no_ht_ht_shepherds(self):
        # When SLURM_HINT=nomultithread and
        # CHPL_RT_NUM_THREADS_PER_LOCALE indicates to use
        # hyperthreading, the number of threads/shepherds should be
        # recuded to the number of cores.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = str(self.sockets * self.cores)
        env['SLURM_HINT'] = 'nomultithread'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.cores * self.threads, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %s Shepherds' % self.cores, proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), proc.stdout)

    def test_10_reserved(self):
        # One core is reserved for the AM handler thread
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES'] = 'true'
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %s Shepherds' % (self.cores - 1), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(0,1), proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getCores(1,1), proc.stdout)

    def test_11_ht_reserved(self):
        # One core is reserved for the AM handler thread when using
        # hyperthreading.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES'] = 'true'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = str(self.sockets * self.cores)
        proc = run("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', proc.stdout)
        self.assertIn('using socket 0', proc.stdout)
        self.assertIn('using socket 1', proc.stdout)
        self.assertIn('Using %d Shepherds' % (self.sockets * self.cores - 2),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getThreads(0,1),
                      proc.stdout)
        self.assertIn("QT_CPUBIND = " + self.getThreads(1,1),
                      proc.stdout)

    @unittest.skip("should not be considered oversubscribed")
    def test_12_oversubscribed_no_env(self):
        # Detect and allow oversubscription w/out CHPL_RT_OVERSUBSCRIBED
        # and CHPL_RT_LOCALES_PER_NODE set.
        env = os.environ.copy()
        env['PMI_MAX_KVS_ENTRIES'] = '20'
        env['PMI_NO_PREINITIALIZE'] = 'y'
        proc = run(['srun', '-l', '--quiet', '--nodes=1', '--ntasks=3',
                    '--ntasks-per-node=3', '--cpus-per-task=16',
                    '--exclusive', '--kill-on-bad-exit',
                    './lps_test_real', '-nl', '3', '-v'], env=env)
        self.assertIn('oversubscribed = True', proc.stdout)
        self.assertIn('Using 8 Shepherds', proc.stdout)

def main(argv):
    global verbose
    failfast = False
    if "-f" in argv:
        failfast = True
        argv.remove("-f")
    if "-v" in argv:
        verbose = True
    unittest.main(argv=argv, failfast=failfast)

if __name__ == '__main__':
    main(sys.argv)

