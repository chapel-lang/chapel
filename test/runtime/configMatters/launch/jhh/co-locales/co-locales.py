#!/usr/bin/env python3

"""
Co-locales command-line argument test.
Usage: ./co-locales.py.
The -v flag prints verbose output, the -f flag will cause testing to stop when
the first test fails.
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

class ColocaleArgs(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Should not be set by default
        if 'CHPL_RT_LOCALES_PER_NODE' in os.environ:
            del os.environ['CHPL_RT_LOCALES_PER_NODE']
        os.environ['CHPL_LAUNCHER_SLURM_VERSION'] = 'slurm'
        os.environ['CHPL_LAUNCHER_CORES_PER_LOCALE'] = '256'
    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)
        self.env = os.environ.copy()

    def runCmd(self, cmd):
        output = runCmd(cmd, self.env);
        return output

    def test_00_base(self):
        """One locale per node"""
        output = self.runCmd("./hello -nl 4 -v --dry-run")
        self.assertTrue('--nodes=4 ' in output or '-N 4 ' in output)
        self.assertIn('--ntasks=4 ', output)
        self.assertNotIn("CHPL_RT_LOCALES_PER_NODE", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_01_1x1(self):
        """Still one locale per node"""
        output = self.runCmd("./hello -nl 1x1 -v --dry-run")
        self.assertTrue('--nodes=1 ' in output or '-N 1 ' in output)
        self.assertIn('--ntasks=1 ', output)
        self.assertIn("CHPL_RT_LOCALES_PER_NODE=1 ", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_02_3x2(self):
        """Three nodes, two locales per node"""
        output = self.runCmd("./hello -nl 3x2 -v --dry-run")
        self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
        self.assertIn('--ntasks=6 ', output)
        self.assertIn("CHPL_RT_LOCALES_PER_NODE=2 ", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_03_1xd1(self):
        """One node, locales-per-node defaults to 1"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '1'
        output = self.runCmd("./hello -nl 1x -v --dry-run")
        self.assertTrue('--nodes=1 ' in output or '-N 1 ' in output)
        self.assertIn('--ntasks=1 ', output)
        self.assertNotIn("CHPL_RT_LOCALES_PER_NODE", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_04_3xd1(self):
        """Three nodes, locales-per-node defaults to 1"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '1'
        output = self.runCmd("./hello -nl 3x -v --dry-run")
        self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
        self.assertIn('--ntasks=3 ', output)
        self.assertNotIn("CHPL_RT_LOCALES_PER_NODE", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_05_3xd2(self):
        """Three nodes, locales-per-node defaults to 2"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        output = self.runCmd("./hello -nl 3x -v --dry-run")
        self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
        self.assertIn('--ntasks=6 ', output)
        self.assertNotIn("CHPL_RT_LOCALES_PER_NODE", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_06_3xd_1(self):
        """Three nodes, locales-per-node is negative"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 3x-1 -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: Number of locales per node must be > 0.")

    def test_07_3xd0(self):
        """Three nodes, locales-per-node is zero"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 3x0 -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: Number of locales per node must be > 0.")

    def test_08_3xZ(self):
        """Three nodes, locales-per-node is not a number"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 3xZ -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            '<command-line arg>:1: error: "Z" is not a valid number of co-locales.')

    def test_09_no_default(self):
        """Three nodes, no locales-per-node default"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 3x -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: CHPL_RT_LOCALES_PER_NODE must be set.")

    def test_10_negative_default(self):
        """Three nodes, locales-per-node default is negative"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '-2'
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 3x -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: CHPL_RT_LOCALES_PER_NODE must be > 0.")

    def test_11_bogus_default(self):
        """Three nodes, locales-per-node default is bogus"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = 'bogus'
        output = self.runCmd("./hello -nl 3x -v --dry-run")
        self.assertIn('warning: CHPL_RT_LOCALES_PER_NODE improper int value "bogus", assuming 1',
                      output)

    def test_12__3x2(self):
        """Negative 3 nodes"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl -3x2 -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: Number of nodes must be > 0.")

    def test_13_0x2(self):
        """Zero nodes"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl 0x2 -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            "<command-line arg>:1: error: Number of nodes must be > 0.")

    def test_14_Zx2(self):
        """Bogus nodes"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl Zx2 -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            '<command-line arg>:1: error: "Z" is not a valid number of nodes.')

    def test_15_env_override(self):
        """Arg overrides CHPL_RT_LOCALES_PER_NODE"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '4'
        output = self.runCmd("./hello -nl 3x2 -v --dry-run")
        self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
        self.assertIn('--ntasks=6 ', output)
        self.assertIn("CHPL_RT_LOCALES_PER_NODE=2 ", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

    def test_16_valid_suffixes(self):
        """Allow valid suffixes"""
        suffixes = {'s':'socket', 'socket':'socket', 'numa':'numa',
                    'llc':'cache', 'c':'core', 'core':'core'}
        for (s, t) in suffixes.items():
            with self.subTest(s=s, t=t):
                output=self.runCmd("./hello -nl 3x2%s -v --dry-run" % s)
                self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
                self.assertIn('--ntasks=6 ', output)
                self.assertIn("CHPL_RT_LOCALES_PER_NODE=2 ", output)
                self.assertIn('CHPL_RT_COLOCALE_OBJ_TYPE=%s ' % t, output)

    def test_17_invalid_suffix(self):
        """Reject invalid suffix"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl -3x2z -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            '<command-line arg>:1: error: "z" is not a valid suffix.')

    def test_18_invalid_suffix2(self):
        """Reject invalid suffix that starts with a valid character"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl -3x2ss -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            '<command-line arg>:1: error: "ss" is not a valid suffix.')

    def test_19_invalid_suffix3(self):
        """Suffix must follow locales-per-node"""
        with self.assertRaises(subprocess.CalledProcessError) as cm:
            output = self.runCmd("./hello -nl -3xs -v --dry-run")
        self.assertEqual(cm.exception.stdout.strip(),
            '<command-line arg>:1: error: "s" is not a valid number of co-locales.')

    def test_20_env_override(self):
        """Arg Mx1 overrides CHPL_RT_LOCALES_PER_NODE"""
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        output = self.runCmd("./hello -nl 3x1 -v --dry-run")
        self.assertTrue('--nodes=3 ' in output or '-N 3 ' in output)
        self.assertIn('--ntasks=3 ', output)
        self.assertIn("CHPL_RT_LOCALES_PER_NODE=1 ", output)
        self.assertNotIn("CHPL_RT_COLOCALE_OBJ_TYPE", output)

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

    setup()

    if len(argv) < 2:
        print('usage: sub_test COMPILER [options]')
        sys.exit(0)

    compiler = argv[1]
    name = os.path.join(getDir(compiler), argv[0])
    del argv[1]

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

