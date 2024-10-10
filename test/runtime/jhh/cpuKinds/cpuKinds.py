#!/usr/bin/env python3

"""
Heterogeneous core tests (different "kinds" of CPUs, in hwloc terminology).
Usage: ./cpuKinds.py. The -v flag prints verbose output, the -f flag will
cause testing to stop on the first test failure.
"""

import unittest
import subprocess
import os
import sys
import shutil
import time


if not 'CHPL_HOME' in os.environ:
    print('CHPL_HOME is not set.')
    sys.exit(1)

import sub_test
import printchplenv

verbose = False
skipReason = None

def runCmd(cmd, env=None, check=True):
    if type(cmd) is str:
        cmd = cmd.split()
    if env is None:
        proc = subprocess.run(cmd, text=True, check=check,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    else:
        proc = subprocess.run(cmd, text=True, check=check, env=env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return proc.stdout

def skipif():
    global skipReason

    # Get the Chapel configuration
    printchplenv.compute_all_values()
    # strip the padding printchplenv puts on some of the keys
    env = {k.strip():v for k,v in printchplenv.ENV_VALS.items()}

    # Verify Chapel configuration
    # It's only necessary to run this on a single locale.
    if env.get('CHPL_COMM', 'none') != 'none':
        skipReason = "CHPL_COMM != none"
        return
    if env.get('CHPL_HWLOC', 'none') == 'none':
        skipReason = "CHPL_HWLOC == none"
        return

class SrunTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pass

    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)
        self.env = os.environ.copy()
        self.env['HWLOC_XMLFILE'] = 'alderLake.xml'
    def runCmd(self, cmd):
        output = runCmd(cmd, env=self.env, check=False)
        return output

    def test_00_base(self):
        """Use all cores on a homogeneous CPU"""
        self.env['HWLOC_XMLFILE'] = 'milan.xml'
        output = self.runCmd("./cpuKinds")
        cpus = " ".join([ str(i) for i in range(0,128)])
        self.assertIn("Physical CPUs: " + cpus, output)
        cpus = " ".join([ str(i) for i in range(0,256)])
        self.assertIn("Logical CPUs: " + cpus, output)

    def test_01_base_mask(self):
        """Mask off one socket to verify that masking works"""
        self.env['HWLOC_XMLFILE'] = 'milan.xml'
        output = self.runCmd("./cpuKinds -m 0-63,128-191")
        cpus = " ".join([ str(i) for i in range(0,64)])
        self.assertIn("Physical CPUs: " + cpus, output)
        cpus = " ".join([ str(i) for i in list(range(0,64)) + \
                        list(range(128,192))])
        self.assertIn("Logical CPUs: " + cpus, output)

    def test_02_performance(self):
        """Use only performance cores"""
        self.env['CHPL_RT_USE_PU_KIND'] = 'performance'
        output = self.runCmd("./cpuKinds")
        self.assertIn("Physical CPUs: 0 2 4 6 8 10 12 14", output)
        self.assertIn("Logical CPUs: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15",
                      output)

    def test_03_efficiency(self):
        """Use only efficiency cores"""
        self.env['CHPL_RT_USE_PU_KIND'] = 'efficiency'
        output = self.runCmd("./cpuKinds")
        self.assertIn("Physical CPUs: 16 17 18 19 20 21 22 23", output)
        self.assertIn("Logical CPUs: 16 17 18 19 20 21 22 23", output)

    def test_04_all(self):
        """Use all cores"""
        self.env['CHPL_RT_USE_PU_KIND'] = 'all'
        output = self.runCmd("./cpuKinds")
        self.assertIn(
            "Physical CPUs: 0 2 4 6 8 10 12 14 16 17 18 19 20 21 22 23",
            output)
        cpus = " ".join([ str(i) for i in range(0,24)])
        self.assertIn("Logical CPUs: " + cpus, output)

    def test_05_bogus(self):
        """Bogus core type"""
        self.env['CHPL_RT_USE_PU_KIND'] = 'bogus'
        output = self.runCmd("./cpuKinds")
        self.assertIn(
            'error: "bogus" is not a valid value for CHPL_RT_USE_PU_KIND.',
            output)
        self.assertIn(
            'Must be one of "performance", "efficiency", or "all".',
            output)

    def test_06_no_accessible(self):
        """All of the desired type of cores are inaccessible"""
        self.env['CHPL_RT_USE_PU_KIND'] = 'performance'
        output = self.runCmd("./cpuKinds -m 16-23")
        self.assertIn('error: No useable cores', output)

    def test_07_default(self):
        """Use performance cores by default"""
        self.env.pop('CHPL_RT_USE_PU_KIND', None)
        output = self.runCmd("./cpuKinds")
        self.assertIn("Physical CPUs: 0 2 4 6 8 10 12 14", output)
        self.assertIn("Logical CPUs: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15",
                      output)

def main(argv):
    global verbose

    startTime=time.time()
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
        print('usage: %s COMPILER [options]' % argv[0])
        sys.exit(0)

    compiler = argv[1]
    del argv[1]
    baseDir = sub_test.get_chpl_base(compiler)
    homeDir = sub_test.get_chpl_home(baseDir)
    testDir = sub_test.get_test_dir(homeDir)
    localDir = sub_test.get_local_dir(testDir)
    name = os.path.join(localDir, argv[0])
    base = os.path.splitext(os.path.basename(argv[0]))[0]

    sub_test.printStartOfTestMsg(time.localtime())
    sub_test.printTestName(name)

    if skipReason is None:

        # Compile the Chapel library
        cmd = compiler + " %s.chpl --library" % base
        if verbose:
            print("Compiling Chapel library")
            print(cmd)
        runCmd(cmd)

        # Compile the C test harness
        cmd = runCmd("compileline --compile") + " %s.c -Llib -l%s " + \
            runCmd("compileline --libraries") + " -o %s"
        cmd = cmd % (base, base, base)
        if verbose:
            print("Compiling test harness")
            print(cmd)
        runCmd(cmd)
    if verbose:
        print("Running tests")
        verbosity=2
    else:
        verbosity=1
    prog = unittest.main(argv=argv, failfast=failfast, exit=False,
                         verbosity=verbosity)

    # Produce output that start_test can parse. To start_test this is a single
    # test. Report report success if all tests succeeded, an error if any
    # test failed, and nothing if all tests were skipped.

    elapsedTime = time.time() - startTime

    if len(prog.result.skipped) > 0:
        sub_test.printSkipping(name, skipReason)
        print("Skipped %d tests in %s" % (len(prog.result.skipped), name))
    if len(prog.result.skipped) != prog.result.testsRun:
        if len(prog.result.errors) > 0 or len(prog.result.failures) > 0:
            print("[Error running tests in %s]" % name)
        else:
            print("[Success matching tests in %s]" % name)
    sub_test.printEndOfTestMsg(name, elapsedTime)
    sub_test.cleanup(base, False)
    shutil.rmtree("./lib", ignore_errors=True)
    sub_test.print_elapsed_sub_test_time(name, elapsedTime)

if __name__ == '__main__':
    main(sys.argv)

