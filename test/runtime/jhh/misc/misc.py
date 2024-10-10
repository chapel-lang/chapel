#!/usr/bin/env python3

"""
Miscellaneous runtime tests
Usage: ./misc.py [options] compiler
The -v flag prints verbose output, the -f flag will cause testing to stop when
the first test fails.
"""

import unittest
import subprocess
import os
import sys
import time
import shutil

if not 'CHPL_HOME' in os.environ:
    print('CHPL_HOME is not set')
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
        # Make sure environment variables are strings, otherwise you get
        # an exception deep in the callstack.
        for (key, value) in env.items():
            if type(value) != str:
                raise Exception('Environment variable "%s" is not a string.' %
                                key)
        proc = subprocess.run(cmd, text=True, check=check, env=env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return proc.stdout

def skipif():
    global skipReason
    global launcher, comm

    # Get the Chapel configuration
    printchplenv.compute_all_values()
    # strip the padding printchplenv puts on some of the keys
    env = {k.strip():v for k,v in printchplenv.ENV_VALS.items()}

    # Verify Chapel configuration

    # Tests requires hwloc
    if env.get('CHPL_HWLOC', 'none') == 'none':
        skipReason = "CHPL_HWLOC == none"
        return

    # Don't test in multi-locale configurations
    if env.get('CHPL_COMM', 'none') != 'none':
        skipReason = "CHPL_COMM != none"
        return

def stringify(lst):
    return " ".join([str(i) for i in lst])

class AccessibleTests(unittest.TestCase):
    """
    Tests handling of inaccessible cores and threads.
    """
    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)
        self.env = os.environ.copy()
        self.env['HWLOC_XMLFILE'] = '../topos/ex2.xml'
        self.sockets = 2
        self.numas = 4
        self.cores = 64
        self.threads = 2

    def runCmd(self, cmd, env=None):
        if env is None:
            env = self.env
        output = runCmd(cmd, env=env, check=False)
        return output

    def getCores(self, base=0):
        return list(range(base, self.sockets * self.cores))

    def getLowThreads(self, base=0):
        return self.getCores(base)

    def getHighThreads(self, base=0):
        return [i+128 for i in self.getCores(base)]

    def getThreads(self, base=0):
        return self.getLowThreads(base) + self.getHighThreads(base)

    def test_00_base(self):
        """
        All cores and threads are accessible.
        """
        output = self.runCmd("./tester")
        cpus = stringify(self.getCores())
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getThreads())
        self.assertIn("Logical CPUs: %s\n" % cpus, output)

    def test_01_high_threads_inaccessible(self):
        """
        All cores and low-numbered threads are accessible.
        """
        output = self.runCmd("./tester -m 0-127")
        cpus = stringify(self.getCores())
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getLowThreads())
        self.assertIn("Logical CPUs: %s\n" % cpus, output)

    def test_02_low_threads_inaccessible(self):
        """
        All cores and high-numbered threads are accessible.
        """
        output = self.runCmd("./tester -m 128-255")
        cpus = stringify(self.getCores())
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getHighThreads())
        self.assertIn("Logical CPUs: %s\n" % cpus, output)

    def test_03_one_core_inaccessible(self):
        """
        All threads are inaccessible on core 0.
        """
        output = self.runCmd("./tester -m 1-127,129-255")
        cpus = stringify(self.getCores(1))
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getThreads(1))
        self.assertIn("Logical CPUs: %s\n" % cpus, output)

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
    os.environ['CHPL_COMPILER'] = compiler
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
        if verbose:
            print("Building test harness")
        runCmd("make clean")
        runCmd("make")
    if verbose:
        print("Running tests")
        verbosity=2
    else:
        verbosity=1
    prog = unittest.main(argv=argv, failfast=failfast, exit=False,
                         verbosity=verbosity)


    """
    Produce output that start_test can parse. To start_test this is a single
    test. Report report success if all tests succeeded, an error if any test
    failed, and nothing if all tests were skipped.
    """

    elapsedTime = time.time() - startTime

    if len(prog.result.skipped) > 0:
        sub_test.printSkipping(name, skipReason)
        print("Skipped %d tests in %s" % (len(prog.result.skipped), name))
    if len(prog.result.skipped) != prog.result.testsRun:
        if len(prog.result.errors) > 0 or len(prog.result.failures) > 0 or \
           len(prog.result.unexpectedSuccesses) > 0:
            print("[Error running tests in %s]" % name)
        else:
            print("[Success matching tests in %s]" % name)
    sub_test.printEndOfTestMsg(name, elapsedTime)
    sub_test.cleanup(base, False)
    runCmd("make clean")
    sub_test.print_elapsed_sub_test_time(name, elapsedTime)

if __name__ == '__main__':
    main(sys.argv)

