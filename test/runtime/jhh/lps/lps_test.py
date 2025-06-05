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

if not 'CHPL_HOME' in os.environ:
    print('CHPL_HOME is not set')
    sys.exit(1)

sys.path.append(os.path.join(os.environ['CHPL_HOME'], 'util', 'chplenv'))
import printchplenv


verbose = False
skipReason = None

def runCmd(cmd, env=None):
    if type(cmd) is str:
        cmd = cmd.split()
    if env is None:
        proc = subprocess.run(cmd, text=True, check=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    else:
        proc = subprocess.run(cmd, text=True, check=True, env=env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return proc.stdout

def setup():
    global skipReason
    global launcher, comm

    # Get the Chapel configuration
    printchplenv.compute_all_values()
    # strip the padding printchplenv puts on some of the keys
    env = {k.strip():v for k,v in printchplenv.ENV_VALS.items()}

    launcher = env.get('CHPL_LAUNCHER')
    comm = env.get('CHPL_COMM')

    # Verify Chapel configuration
    # These tests only run on comm=ofi and slurm-srun
    launchers = ['slurm-srun', 'slurm-gasnetrun_ibv']
    if launcher not in launchers:
        skipReason = "CHPL_LAUNCHER not one of " + str(launchers)
        return
    comms = ['ofi', 'gasnet']
    if comm not in comms:
        skipReason = "CHPL_COMM not one of " + str(comms)
        return

class LocalePerSocket(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        global verbose
        if skipReason is not None:
            return
        v = runCmd("sinfo --version").split()[1]
        fields = v.split('.')
        major = int(fields[0])
        if major < 23:
            # older versions of sinfo do not support %i
            fmt = "%X %Y %Z"
        else:
            fmt = "%X %Y %Z %i"
        cmd = ["sinfo", "--format=" + fmt, "--noheader", "--exact"]
        partition = os.environ.get('CHPL_LAUNCHER_PARTITION')
        if partition is not None:
            cmd += ["--partition", partition]
            if verbose:
                print("Partition: ", partition)

        output = runCmd(cmd)
        for line in output.splitlines():
            fields = line.split()
            # skip lines with reservation info
            if len(fields) == 3:
                (cls.sockets, cls.cores, cls.threads) = \
                    [int(i) for i in fields]
                break

        if verbose:
            print("Sockets: ", cls.sockets)
            print("Cores/Socket: ", cls.cores)
            print("Threads/Core: ", cls.threads)

        # We need the qthread output
        os.environ['QTHREAD_INFO'] = '2'

        # Should not be set by default
        if 'CHPL_RT_LOCALES_PER_NODE' in os.environ:
            del os.environ['CHPL_RT_LOCALES_PER_NODE']

    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)

    def runCmd(self, cmd, env = None):
        return runCmd(cmd, env)

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
        self.assertTrue('--nodes=%d' % nodes in output or '-N %d' % nodes in output)
        self.assertIn('--ntasks=%d' % tasks, output)
        if launcher != 'slurm-gasnetrun_ibv':
            self.assertIn('--cpus-per-task=%d' % cpusPerTask, output)
            if (cpuBind != None):
                self.assertIn('--cpu-bind=%s' % cpuBind, output)
            else:
                self.assertNotIn('--cpu-bind', output)
        self.assertNotIn('--ntasks-per-node', output)

    def mrAllocatedRequired(self):
        output = runCmd("srun fi_info -v")
        for line in output.splitlines():
            if "mr_mode" in line:
                if "FI_MR_ALLOCATED" in line:
                    return True
                else:
                    break
        return False

    def test_00_base(self):
        global cls
        output = self.runCmd("./lps_test -nl 2 -v")
        self.checkArgs(2, 2, 1, self.sockets * self.cores * self.threads,
                       None, output)
        self.assertIn('oversubscribed = False', output)
        self.assertIn('Using %d Shepherds' % (self.sockets * self.cores),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"),
                      output)

    def test_01_oversubscribed_env(self):
        # Being oversubscribed should have no effect on the number
        # of shepherds.
        if self.mrAllocatedRequired():
            self.skipTest("FI_MR_ALLOCATED is required by provider")
        env = os.environ.copy()
        env['CHPL_RT_OVERSUBSCRIBED'] = 'true'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(2, 2, 1, self.sockets * self.cores * self.threads,
                       None, output)
        self.assertIn('oversubscribed = True', output)
        self.assertIn('Using %s Shepherds' % (self.sockets * self.cores),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"), output)

    def test_02_two_lpn(self):
        # One locale per socket. Each locale only uses the cores in
        # its socket.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %s Shepherds' % self.cores, output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), output)

    def test_03_four_lpn(self):
        # Four locales per node is an error if CHPL_RT_LOCALES_PER_NODE
        # is set.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '4'
        output = self.runCmd(['./lps_test', '-nl', '4', '-v'], env=env)
        self.assertIn('error: The number of locales on the node is '
                      'greater than the number of sockets (4 > 2).',
                      output)

    def test_04_one_locale(self):
        # One locale uses only the cores in its socket.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        output = self.runCmd(['./lps_test', '-nl', '1', '-v'], env=env)
        self.checkArgs(1, 1, 2, self.sockets * self.cores * self.threads,
                       'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('Using %s Shepherds' % self.cores, output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), output)

    def test_05_two_lpn_oversubscribed(self):
        # CHPL_RT_OVERSUBSCRIBED should have no effect when there is
        # one locale per socket
        if self.mrAllocatedRequired():
            self.skipTest("FI_MR_ALLOCATED is required by provider")
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_OVERSUBSCRIBED'] = 'true'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('oversubscribed = True', output)
        self.assertIn('Using %s Shepherds' % self.cores, output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), output)

    def test_06_no_ht(self):
        # If SLURM_HINT=nomultithread then the launcher should request
        # only one PU per core and the locales should only use the
        # same.
        env = os.environ.copy()
        env['SLURM_HINT'] = 'nomultithread'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(2, 2, 1, self.sockets * self.cores, None, output)
        self.assertNotIn('using socket', output)
        self.assertNotIn('using socket', output)
        self.assertIn('Using %s Shepherds' % (self.sockets * self.cores),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getCores("all"), output)

    def test_07_two_lpn_no_ht(self):
        # If SLURM_HINT=nomultithread then each locale should only use one
        # PU per core.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['SLURM_HINT'] = 'nomultithread'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %s Shepherds' % self.cores, output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), output)

    def test_08_ht_shepherds(self):
        # One worker per PU with hyperthreading.
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = \
            '%s' % (self.sockets * self.cores)
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %s Shepherds' % (self.cores * self.threads),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getThreads(0), output)
        self.assertIn("QT_CPUBIND = " + self.getThreads(1), output)

    def test_09_no_ht_ht_shepherds(self):
        # When SLURM_HINT=nomultithread and
        # CHPL_RT_NUM_THREADS_PER_LOCALE indicates to use
        # hyperthreading, the number of threads/shepherds should be
        # reduced to the number of cores.
        if launcher == 'slurm-gasnetrun_ibv':
            self.skipTest('slurm-gasnetrun_ibv does not implement SLURM_HINT=nomultithread')
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = str(self.sockets * self.cores)
        env['SLURM_HINT'] = 'nomultithread'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.cores * self.threads, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %s Shepherds' % self.cores, output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(1), output)

    def test_10_reserved(self):
        # One core is reserved for the AM handler thread
        if comm == 'gasnet':
            self.skipTest("CHPL_COMM=gasnet does not support dedicated progress thread")
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES'] = 'true'
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %s Shepherds' % (self.cores - 1), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(0,1), output)
        self.assertIn("QT_CPUBIND = " + self.getCores(1,1), output)

    def test_11_ht_reserved(self):
        # One core is reserved for the AM handler thread when using
        # hyperthreading.
        if comm == 'gasnet':
            self.skipTest("CHPL_COMM=gasnet does not support dedicated progress thread")
        env = os.environ.copy()
        env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        env['CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES'] = 'true'
        env['CHPL_RT_NUM_THREADS_PER_LOCALE'] = str(self.sockets * self.cores)
        output = self.runCmd("./lps_test -nl 2 -v", env=env)
        self.checkArgs(1, 2, 2, self.sockets * self.cores, 'none', output)
        self.assertIn('using socket 0', output)
        self.assertIn('using socket 1', output)
        self.assertIn('Using %d Shepherds' % (self.sockets * self.cores - 2),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getThreads(0,1),
                      output)
        self.assertIn("QT_CPUBIND = " + self.getThreads(1,1),
                      output)

    @unittest.skip("should not be considered oversubscribed")
    def test_12_oversubscribed_no_env(self):
        # Detect and allow oversubscription w/out CHPL_RT_OVERSUBSCRIBED
        # and CHPL_RT_LOCALES_PER_NODE set.
        env = os.environ.copy()
        env['PMI_MAX_KVS_ENTRIES'] = '20'
        env['PMI_NO_PREINITIALIZE'] = 'y'
        output = self.runCmd(['srun', '-l', '--quiet', '--nodes=1',
                    '--ntasks=3',
                    '--ntasks-per-node=3', '--cpus-per-task=16',
                    '--exclusive', '--kill-on-bad-exit',
                    './lps_test_real', '-nl', '3', '-v'], env=env)
        self.assertIn('oversubscribed = True', output)
        self.assertIn('Using 8 Shepherds', output)

# copied from sub_test.py
# report an error message and exit
def Fatal(message):
    sys.stdout.write('[Error (sub_test): '+message+']\n')
    magic_exit_code = reduce(operator.add, map(ord, 'CHAPEL')) % 256
    sys.exit(magic_exit_code)

def getDir(compiler):

    # This code is copied from sub_test.py to ensure we print
    # the path of the test in the same format so all paths look
    # the same in the test outputs.

    # Find the base installation
    #compiler=argv[1]
    if not os.access(compiler,os.R_OK|os.X_OK):
        Fatal('Cannot execute compiler \''+compiler+'\'')

    is_chpldoc = compiler.endswith('chpldoc')

    path_to_compiler=os.path.abspath(os.path.dirname(compiler))
    # Assume chpl binary is 2 directory levels down in the base installation
    (chpl_base, tmp) = os.path.split(path_to_compiler)
    (chpl_base, tmp) = os.path.split(chpl_base)
    chpl_base=os.path.normpath(chpl_base)
    # sys.stdout.write('CHPL_BASE='+chpl_base+'\n')

    # If $CHPL_HOME is not set, use the base installation of the compiler

    chpl_home=os.getenv('CHPL_HOME', chpl_base)
    chpl_home=os.path.normpath(chpl_home)

    # Find the test directory
    testdir=chpl_home+'/test'
    if os.path.isdir(testdir)==0:
        testdir=chpl_home+'/examples'
        if os.path.isdir(testdir)==0:
            Fatal('Cannot find test directory '+chpl_home+'/test or '+testdir)
    # Needed for MacOS mount points
    testdir = os.path.realpath(testdir)
    # sys.stdout.write('testdir='+testdir+'\n')

    # If user specified a different test directory (e.g. with --test-root flag on
    # start_test), use it instead.
    test_root_dir = os.environ.get('CHPL_TEST_ROOT_DIR')
    if test_root_dir is not None:
        testdir = test_root_dir

    # Get the current directory (normalize for MacOS case-sort-of-sensitivity)
    localdir = os.path.normpath(os.getcwd()).replace(testdir, '.')
    # sys.stdout.write('localdir=%s\n'%(localdir))

    if localdir.find('./') == 0:
        # strip off the leading './'
        localdir = localdir.lstrip('.')
        localdir = localdir.lstrip('/')
    # sys.stdout.write('localdir=%s\n'%(localdir))

    return localdir

def main(argv):
    global verbose
    failfast = False
    if "-f" in argv or "--failfast" in argv:
        failfast = True
        try:
            argv.remove("-f")
            argv.remove("--failfast")
        except ValueError:
            pass
    if "-v" in argv or "--verbose" in argv:
        verbose = True

    setup()

    if len(argv) < 2:
        print('usage: sub_test COMPILER [options]')
        sys.exit(0)

    compiler = argv[1]
    name = os.path.join(getDir(compiler), argv[0])
    del argv[1]

    if skipReason is None:
        # Compile the test program
        cmd = compiler + ' lps_test.chpl'
        if verbose:
            print("Compiling test program")
            print(cmd)
        runCmd(cmd)
    if verbose:
        print("Running tests")

    prog = unittest.main(argv=argv, failfast=failfast, exit=False)

    # Produce output that start_test can parse. To start_test this is a single
    # test. Report report success if all tests succeeded, an error if any
    # test failed, and nothing if all tests were skipped.

    if len(prog.result.skipped) > 0:
        print("Skipped %d tests in %s" % (len(prog.result.skipped), name))
    if len(prog.result.skipped) != prog.result.testsRun:
        if len(prog.result.errors) > 0 or len(prog.result.failures) > 0:
            print("[Error running tests in %s]" % name)
        else:
            print("[Success matching tests in %s]" % name)

if __name__ == '__main__':
    main(sys.argv)

