#!/usr/bin/env python3

"""
Co-locale tests. Usage: ./colocales.py. The -v flag prints
verbose output, the -f flag will cause testing to stop when the
first test fails.
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
    # It's only necessary to run this on a single locale.
    if env.get('CHPL_COMM', 'none') != 'none':
        skipReason = "CHPL_COMM != none"
        return
    if env.get('CHPL_HWLOC', 'none') == 'none':
        skipReason = "CHPL_HWLOC == none"
        return

def stringify(lst):
    return " ".join([str(i) for i in lst])

"""
This class defines utility functions and the test cases (which start
with 'test_'). The tests in this class are not run directly, but instead are
run from subclasses that use different machine topologies. The outer class
'TestCases' prevents the unittest framework from trying to run the test cases
in TestCase.
"""
class TestCases(object):
    class TestCase(unittest.TestCase):
        def setUp(self):
            if skipReason is not None:
                self.skipTest(skipReason)
            self.env = os.environ.copy()

        def runCmd(self, cmd, env=None):
            if env is None:
                env = self.env
            output = runCmd(cmd, env=env, check=False);
            return output

        # Returns a list of cores in the specified partition
        def getCores(self,  index, partitions):
            if index == "all":
                return sorted([x for lst in [self.getCores(i, partitions)
                              for i in range(0,partitions)] for x in lst])
            else:
                coresPerPartition = int((self.cores * self.sockets) /
                                        partitions)
                return [i + (index * coresPerPartition)
                                for i in range(0,coresPerPartition)]

        # Returns a list of cores in the specified socket.
        def getSocketCores(self, socket):
            return self.getCores(socket, self.sockets)

        # Returns a list of threads (PUs) in the specified partition
        def getThreads(self, index, partitions):
            if index == "all":
                return sorted([x for lst in [self.getThreads(i, partitions)
                            for i in range(0,partitions)] for x in lst])
            cores = self.getCores(index, partitions)
            threads = []
            for t in range(0, self.threads):
                threads += [cores[i] + (t * self.sockets * self.cores)
                            for i in range(0, len(cores))]
            return threads

        # Returns a list of threads in the specified socket
        def getSocketThreads(self, socket):
            return self.getThreads(socket, self.sockets)

        """
        One locale, should have access to all cores and threads and use the
        suggested NIC.
        """
        def test_00_base(self):
            output = self.runCmd("./colocales -N %s" % self.nics[0])
            cpus = " ".join([str(i) for i in self.getSocketCores("all")])
            self.assertIn("Physical CPUs: " + cpus, output)
            cpus = " ".join([str(i) for i in self.getSocketThreads("all")])
            self.assertIn("Logical CPUs: " + cpus, output)
            self.assertIn("NIC: " + self.nics[0], output)

        """
        One locale, should have access to all cores and threads and use
        another NIC when it is suggested.
        """
        def test_01_use_another_NIC(self):
            if (len(self.nics) == 1):
                self.skipTest("only one NIC")
            output = self.runCmd("./colocales -N %s" % self.nics[1])
            self.assertIn("NIC: " + self.nics[1], output)

        """
        Locale in socket should use cores and threads in that socket and
        the closest NIC.
        """
        def test_02_in_socket(self):
            env = self.env.copy()
            env['CHPL_RT_LOCALES_PER_NODE'] = str(self.sockets)
            for i in range(0, self.sockets):
                with self.subTest(i=i):
                    output = self.runCmd("./colocales -r %d -n %d -N %s" %
                                     (i, self.sockets, self.nicForSocket[i]),
                                     env=env)
                    cpus = stringify(self.getSocketCores(i))
                    self.assertIn("Physical CPUs: %s\n" % cpus, output)
                    cpus = stringify(self.getSocketThreads(i))
                    self.assertIn("Logical CPUs: %s\n" % cpus, output)
                    self.assertIn("NIC: " + self.nicForSocket[i], output)


"""
These classes represent different machine topologies based on real machines.
The topologies are stored in the XML files. We could extract the number of
sockets, cores, etc., from those files, but for now we just hard-code that
information because there are only a few configurations.
"""

"""
HPE Cray EX. Two sockets, four NUMA domains per socket, 64 cores per socket,
two threads per core, and one NIC per socket.
"""
class Ex2Tests(TestCases.TestCase):
    def setUp(self):
        super().setUp()
        self.env['HWLOC_XMLFILE'] = 'ex2.xml'
        self.sockets = 2
        self.numas = 4
        self.cores = 64
        self.threads = 2
        self.nics= ['0000:21:00.0', '0000:a1:00.0']
        self.nicForSocket = ['0000:21:00.0', '0000:a1:00.0']

"""
AWS hpc6a.48xlarge instance. Two sockets, two NUMA domains per socket, 48
cores per socket, one thread per core, and one NIC not in a socket.
domains.
"""
class Hpc6aTests(TestCases.TestCase):
    def setUp(self):
        super().setUp()
        self.env['HWLOC_XMLFILE'] = 'hpc6a.48xlarge.xml'
        self.sockets = 2
        self.numas = 2
        self.cores = 48
        self.threads = 1
        self.nics= ['0000:00:06.0']
        self.nicForSocket = ['0000:00:06.0', '0000:00:06.0']

# AWS m6in-dy-m6in32xlarge instance. Two sockets, one NUMA domain per socket,
# 32 cores per socket, one thread per core, and two NICs not in sockets.

class M6inTests(TestCases.TestCase):
    def setUp(self):
        super().setUp()
        self.env['HWLOC_XMLFILE'] = 'm6in-dy-m6in32xlarge.xml'
        self.sockets = 2
        self.numas = 1
        self.cores = 32
        self.threads = 2
        self.nics= ['0000:00:06.0', '0000:00:08.0']

    """
    The base test_02_in_socket test doesn't work because the NICs are not in
    sockets in this topology. Instead, check that the two locales use
    different NICs.
    """
    def test_02_in_socket(self):
        self.env['CHPL_RT_LOCALES_PER_NODE'] = '2'
        output = self.runCmd("./colocales -r 0 -n 2 -N %s" % self.nics[0])
        for line in output.split('\n'):
            if line.startswith("NIC:"):
                nic0 = line.split()[1]
                self.assertIn(nic0, self.nics)
                break
        output = self.runCmd("./colocales -r 1 -n 2 -N %s" % self.nics[0])
        for line in output.split('\n'):
            if line.startswith("NIC:"):
                nic1 = line.split()[1]
                self.assertIn(nic1, self.nics)
                self.assertNotEqual(nic0, nic1)
                break

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
    localDir = sub_test.get_local_dir(sub_test.get_chpl_base(compiler))
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
        if len(prog.result.errors) > 0 or len(prog.result.failures) > 0:
            print("[Error running tests in %s]" % name)
        else:
            print("[Success matching tests in %s]" % name)
    sub_test.printEndOfTestMsg(name, elapsedTime)
    sub_test.cleanup(base, False)
    runCmd("make clean")
    sub_test.print_elapsed_sub_test_time(name, elapsedTime)

if __name__ == '__main__':
    main(sys.argv)

