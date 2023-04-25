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
    # These tests only run on slurm-srun
    output = runCmd("printchplenv --simple")
    for line in output.splitlines():
        (key, value) = line.split('=',1)
        if key == 'CHPL_LAUNCHER' and value != 'slurm-srun':
            skipReason = "CHPL_LAUNCHER != slurm-srun"
    # Verify environment variables
    output = runCmd("printenv")
    for line in output.splitlines():
        (key, value) = line.split('=',1)
        if key == 'CHPL_RT_LOCALES_PER_NODE' and value != 1:
            skipReason = "CHPL_RT_LOCALES_PER_NODE != 1"
        if key == 'SLURM_HINT' and value == 'nomultithread':
            skipReason = "SLURM_HINT == nomultithread"

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
        try:
            (_, batch) = output.split();
        except:
            print(output);
        with open(batch) as fd:
            output = fd.read();
        os.unlink(batch)
        return output

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

    # Add sbatch and srun to our path
    path = os.environ['PATH']
    os.environ['PATH'] = os.path.join(os.getcwd(), "bin") + ":" + path

    # Compile the test program
    cmd = 'chpl hello.chpl'
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
        print("Skipped %d tests" % len(prog.result.skipped))
    if len(prog.result.skipped) != prog.result.testsRun:
        if len(prog.result.errors) > 0 or len(prog.result.failures) > 0:
            print("[Error running tests]")
        else:
            print("[Success matching test results]")

if __name__ == '__main__':
    main(sys.argv)

