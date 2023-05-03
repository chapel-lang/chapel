#!/usr/bin/env python3

"""
Slurm reservation tests. Usage: ./reservation.py. The -v flag prints
verbose output, the -f flag will cause testing to stop when the
first test fails.
"""

import unittest
import subprocess
import os
import sys
import operator
from functools import reduce


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

def skipif():
    global skipReason

    # Get the Chapel configuration
    printchplenv.compute_all_values()
    # strip the padding printchplenv puts on some of the keys
    env = {k.strip():v for k,v in printchplenv.ENV_VALS.items()}

    # Verify Chapel configuration
    if env.get('CHPL_LAUNCHER', None) != 'slurm-srun':
        skipReason = "CHPL_LAUNCHER != slurm-srun"
        return

    # Verify environment
    if os.environ.get('CHPL_RT_LOCALES_PER_NODE', '1') != '1':
        skipReason = "CHPL_RT_LOCALES_PER_NODE != 1"
        return
    if os.environ.get('SLURM_HINT') == 'nomultithread':
        skipReason = "SLURM_HINT == nomultithread"
        return

class SrunTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pass;

    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)
        self.env = os.environ.copy()
        try:
            del self.env['CHPL_LAUNCHER_PARTITION']
            del self.env['CHPL_LAUNCHER_USE_SBATCH']
        except:
            pass

    def runCmd(self, cmd):
        output = runCmd(cmd, self.env);
        return output

    def test_00_base(self):
        """No partition nor reservation"""
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=256', output)

    # Partition tests, no reservation

    def test_01_part0(self):
        """SLURM_PARTITION specifies a partition"""
        self.env['SLURM_PARTITION'] = "foo"
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=128', output)
        self.assertIn('--partition=foo', output)

    def test_02_part1(self):
        """SLURM_PARTITION specifies a different partition"""
        self.env['SLURM_PARTITION'] = "bar"
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=96', output)
        self.assertIn('--partition=bar', output)

    def test_03_part2(self):
        """CHPL_LAUNCHER_PARTITION has precedence"""
        self.env['SLURM_PARTITION'] = "bar"
        self.env['CHPL_LAUNCHER_PARTITION'] = "foo"
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=128', output)
        self.assertIn('--partition=foo', output)

    def test_04_part3(self):
        """--partition has precedence"""
        self.env['CHPL_LAUNCHER_PARTITION'] = "bar"
        output = self.runCmd("./hello -nl 1 -v --dry-run --partition=foo")
        self.assertIn('--cpus-per-task=128', output)
        self.assertIn('--partition=foo', output)

    def test_05_part4(self):
        """SLURM_JOB_PARTITION has precedence"""
        self.env['SLURM_JOB_PARTITION'] = "foo"
        output = self.runCmd("./hello -nl 1 -v --dry-run --partition=bar")
        self.assertIn('--cpus-per-task=128', output)
        self.assertIn('--partition=foo', output)

    # Reservation tests

    def test_06_res_foo(self):
        """Inside a reservation"""
        self.env['SLURM_JOB_PARTITION'] = "foo"
        self.env['SLURM_RESERVATION'] = "res_foo"
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=64', output)
        self.assertIn('--partition=foo', output)

    def test_07_res_bar(self):
        """Inside a different reservation"""
        self.env['SLURM_JOB_PARTITION'] = "bar"
        self.env['SLURM_RESERVATION'] = "res_bar"
        output = self.runCmd("./hello -nl 1 -v --dry-run")
        self.assertIn('--cpus-per-task=48', output)
        self.assertIn('--partition=bar', output)

class SbatchTests(SrunTests):

    def setUp(self):
        super().setUp()
        self.env['CHPL_LAUNCHER_USE_SBATCH'] = 'true'

    def runCmd(self, cmd):
        """Use the contents of the batch file as output"""
        output = super().runCmd(cmd)
        batch = output.split()[-1]
        with open(batch) as fd:
            output = fd.read();
        os.unlink(batch)
        return output

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

    chpl_home=os.getenv('CHPL_HOME', chpl_base);
    chpl_home=os.path.normpath(chpl_home)

    # Find the test directory
    testdir=chpl_home+'/test'
    if os.path.isdir(testdir)==0:
        testdir=chpl_home+'/examples'
        if os.path.isdir(testdir)==0:
            Fatal('Cannot find test directory '+chpl_home+'/test or '+testdir)
    # Needed for MacOS mount points
    testdir = os.path.realpath(testdir)
    # sys.stdout.write('testdir='+testdir+'\n');

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
    if "-f" in argv or "--force" in argv:
        failfast = True
        try:
            argv.remove("-f")
            argv.remove("--force")
        except:
            pass
    if "-v" in argv or "--verbose" in argv:
        verbose = True

    skipif()

    if len(argv) < 2:
        print('usage: sub_test COMPILER [options]')
        sys.exit(0)

    compiler = argv[1]
    name = os.path.join(getDir(compiler), argv[0])
    del argv[1]

    # Add sbatch and srun to our path
    path = os.environ['PATH']
    os.environ['PATH'] = os.path.join(os.getcwd(), "bin") + ":" + path

    if skipReason is None:
        # Compile the test program
        cmd = compiler + ' hello.chpl'
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

