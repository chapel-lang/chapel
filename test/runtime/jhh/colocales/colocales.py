#!/usr/bin/env python3

"""
Co-locale tests
Usage: ./colocales.py [options] compiler
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

class Socket(object):
    def __init__(self, config, index, totalCores):
        self.cores = []
        self.threads = []
        if config.coresPerSocket > 0:
            offset = index * config.coresPerSocket
            self.cores = range(offset, offset + config.coresPerSocket)
            for t in range(0, config.threadsPerCore):
                self.threads += [self.cores[i] + (t * totalCores)
                                 for i in range(0, len(self.cores))]
            self.threads.sort()


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

class TestFramework(unittest.TestCase):
    """
    This class defines utility functions and the test cases (which start
    with 'test_'). The tests in this class are not run directly, but
    instead are run from subclasses that use different machine
    topologies.
    """
    def setUp(self):
        if skipReason is not None:
            self.skipTest(skipReason)
        self.env = os.environ.copy()

    def runCmd(self, cmd, env=None):
        if env is None:
            env = self.env
        output = runCmd(cmd, env=env, check=False)
        return output

    # Returns a list of cores in the specified partition
    def getCores(self,  index, numPartitions):
        coresPerPartition = int(len(self.cores) / numPartitions)
        offset = index * coresPerPartition
        return self.cores[offset:offset + coresPerPartition]

    # Returns a list of cores in the specified NUMA domain
    def getNumaCores(self, numa):
        return self.getCores(numa, len(self.sockets) * self.numas)

    # Returns a list of cores in the specified L3 cache
    def getCacheCores(self, cache):
        return self.getCores(cache, len(self.sockets) * self.caches)

    # Returns a list of threads (PUs) in the specified partition
    def getThreads(self, index, partitions):
        cores = self.getCores(index, partitions)
        threads = []
        for t in range(0, self.threadsPerCore):
            threads += [cores[i] + (t * len(self.cores))
                        for i in range(0, len(cores))]
        return threads

    # Returns a list of threads in the specified socket

    # Returns a list of threads in the specified NUMA domain
    def getNumaThreads(self, numa):
        return self.getThreads(numa, len(self.sockets) * self.numas)

    # Returns a list of threads in the specified L3 cache
    def getCacheThreads(self, cache):
        return self.getThreads(cache, len(self.sockets) * self.caches)

    def in_socket_test(self, socket, nic):
        """
        Locale in socket should use cores and threads in that socket and
        the closest NIC, no matter which NIC is suggested.
        """
        output = self.runCmd("./colocales -r %d -n %d -N %s" %
                         (socket, len(self.sockets), nic))
        cpus = stringify(self.sockets[socket].cores)
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.sockets[socket].threads)
        self.assertIn("Logical CPUs: %s\n" % cpus, output)
        found = False
        for s in self.nicsForSocket[socket]:
            if "NIC: " + s in output:
                found = True
                break
        self.assertTrue(found, "assigned NIC is in the same socket")

    def in_numa_test(self, numa, nic):
        """
        Locale in NUMA should use cores and threads in that NUMA and
        the closest NIC, no matter which NIC is suggested.
        """
        numas = self.numas * len(self.sockets)
        output = self.runCmd("./colocales -r %d -n %d -N %s" %
                         (numa, numas, nic))
        cpus = stringify(self.getNumaCores(numa))
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getNumaThreads(numa))
        self.assertIn("Logical CPUs: %s\n" % cpus, output)
        self.assertIn("NIC: " + self.nicForNuma[numa], output)


    def in_cache_test(self, cache, nic):
        """
        Locale in L3 cache should use cores and threads in that cache and
        the closest NIC, no matter which NIC is suggested.
        """
        caches = self.caches * len(self.sockets)
        output = self.runCmd("./colocales -r %d -n %d -N %s" %
                         (cache, caches, nic))
        cpus = stringify(self.getCacheCores(cache))
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getCacheThreads(cache))
        self.assertIn("Logical CPUs: %s\n" % cpus, output)
        self.assertIn("NIC: " + self.nicForCache[cache], output)

    def in_core_test(self, core, nic):
        """
        Locale in a core should use only that core and its threads and
        the closest NIC, no matter which NIC is suggested.
        """
        numCores = len(self.cores)
        output = self.runCmd("./colocales -r %d -n %d -N %s" %
                         (core, numCores, nic))
        cpus = stringify(self.getCores(core, numCores))
        self.assertIn("Physical CPUs: %s\n" % cpus, output)
        cpus = stringify(self.getThreads(core, numCores))
        self.assertIn("Logical CPUs: %s\n" % cpus, output)
        self.assertIn("NIC: " + self.nicForCore[core], output)

class TestCases(object):
    """
    This outer class prevents the unittest framework from running the
    test cases in TestCase.
    """
    class TestCase(TestFramework):
        def test_00_base(self):
            """
            One locale, should have access to all cores and threads and use
            the suggested NIC.
            """
            if len(self.nics) > 0:
                output = self.runCmd("./colocales -N %s" % self.nics[0])
                self.assertIn("NIC: " + self.nics[0], output)
            else:
                output = self.runCmd("./colocales")
            cpus = stringify(self.cores)
            self.assertIn("Physical CPUs: %s\n" % cpus, output)
            cpus = stringify(self.threads)
            self.assertIn("Logical CPUs: %s\n" % cpus, output)

        def test_01_use_another_NIC(self):
            """
            One locale, should have access to all cores and threads and use
            another NIC when it is suggested.
            """
            if (len(self.nics) < 2):
                self.skipTest("fewer than two NICs")
            output = self.runCmd("./colocales -N %s" % self.nics[1])
            self.assertIn("NIC: " + self.nics[1], output)

        def test_02_suggest_nic_in_socket(self):
            """
            Locale should use NIC in its socket when that is suggested.
            """
            if not hasattr(self, 'nicsForSocket'):
                self.skipTest("NICs are not in sockets")
            for i in range(0, len(self.sockets)):
                for n in self.nicsForSocket[i]:
                    with self.subTest(i=i, n=n):
                        self.in_socket_test(i, n)

        def test_03_suggest_nic_in_other_socket(self):
            """
            Locale should use NIC in its socket when it isn't suggested.
            """
            if not hasattr(self, 'nicsForSocket'):
                self.skipTest("NICs are not in sockets")
            if (len(self.nics) == 1):
                self.skipTest("only one NIC")
            for i in range(0, len(self.sockets)):
                for n in self.nicsForSocket[(i+1)%len(self.sockets)]:
                    with self.subTest(i=i, n=n):
                        self.in_socket_test(i, n)

        def test_04_suggest_nic_in_numa(self):
            """
            Locale should use NIC in its NUMA when it is suggested.
            """
            if not hasattr(self, 'nicForNuma'):
                self.skipTest("NICs are not in NUMAs")
            for i in range(0, self.numas * len(self.sockets)):
                with self.subTest(i=i):
                    self.in_numa_test(i, self.nicForNuma[i])

        def test_05_suggest_nic_in_other_numa(self):
            """
            Locale should use NIC in its NUMA when it isn't suggested.
            """
            if not hasattr(self, 'nicForNuma'):
                self.skipTest("NICs are not in NUMAs")
            if (len(self.nics) == 1):
                self.skipTest("only one NIC")
            numas = self.numas * len(self.sockets)
            for i in range(0, numas):
                with self.subTest(i=i):
                    self.in_numa_test(i, self.nicForNuma[(i+1)%numas])

def setXML(obj, xml):
    if not os.path.exists(xml):
        raise Exception("Missing XML file: %s" % xml)
    obj.env['HWLOC_XMLFILE'] = xml

"""
These classes represent different machine topologies based on real machines.
The topologies are stored in the XML files. We could extract the number of
sockets, cores, etc., from those files, but for now we just hard-code that
information because there are only a few configurations.
"""

def setupEx2(obj):
    setXML(obj, 'ex2.xml')
    obj.coresPerSocket = 64
    obj.threadsPerCore = 2
    obj.sockets = [Socket(obj, i, 128) for i in range(0,2)]
    obj.cores = sorted([core for s in obj.sockets for core in s.cores])
    obj.threads = sorted([thread for s in obj.sockets
                          for thread in s.threads])
    obj.numas = 4
    obj.caches = 8
    obj.nics = ['0000:21:00.0', '0000:a1:00.0']
    obj.nicsForSocket = [['0000:21:00.0'], ['0000:a1:00.0']]
    obj.nicForNuma = ['0000:21:00.0', '0000:21:00.0', '0000:21:00.0',
                      '0000:21:00.0', '0000:a1:00.0', '0000:a1:00.0',
                      '0000:a1:00.0', '0000:a1:00.0']
    obj.nicForCache = [obj.nics[int(i/obj.caches)]
                        for i in range(0, len(obj.sockets) * obj.caches)]
    obj.nicForCore = [obj.nics[int(i/obj.coresPerSocket)]
                        for i in range(0, len(obj.cores))]

class Ex2Tests(TestCases.TestCase):
    """
    HPE Cray EX. Two sockets, four NUMA domains per socket, 64 cores per
    socket, two threads per core, and one NIC per socket.
    """
    def setUp(self):
        super().setUp()
        setupEx2(self)

    def test_10_generalized(self):
        """
        16 co-locales should each have 8 cores
        """
        for i in range(0, 16):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -n %d -N %s" %
                                     (i, 16, self.nics[0]))
                cpus = stringify([i*8+j for j in range(0, 8)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)

    def test_11_not_enough_cores(self):
        """
        Must have at least one core per locale
        """
        output = self.runCmd("./colocales -r 0 -n 129")
        self.assertIn("error: Cannot run 129 co-locales on 128 cores.\n",
                      output)

    def test_12_leftover_cores(self):
        """
        Warn about unused cores
        """
        output = self.runCmd("./colocales -r 0 -n 17")
        self.assertIn("warning: 9 cores are unused\n", output)

    def test_13_remainder_node(self):
        """
        Remainder node has fewer co-locales.
        However, they should not get more than their allotment of cores.
        """
        for i in range(0, 3):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -n %d -a 3 -N %s" %
                                     (i, 16, self.nics[0]))
                cpus = stringify([i*8+j for j in range(0, 8)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)

    def test_14_oversubscribed(self):
        """
        All locales get all cores when oversubscribed.
        """
        for i in range(0, 4):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -a 4 -N %s" %
                                     (i, self.nics[0]))
                cpus = stringify(self.cores)
                self.assertIn("Physical CPUs: %s\n" % cpus, output)
                cpus = stringify(self.threads)
                self.assertIn("Logical CPUs: %s\n" % cpus, output)


class Ex3Tests(TestCases.TestCase):
    """
    HPE Cray EX. One sockets, four NUMA domains per socket, 64 cores per
    socket, two threads per core, one NIC per NUMA domain, and one GPU
    per NUMA domain.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'ex3.xml')
        self.coresPerSocket = 64
        self.threadsPerCore = 2
        self.sockets = [Socket(self, 0, 64)]
        self.cores = sorted([core for s in self.sockets for core in s.cores])
        self.threads = sorted([thread for s in self.sockets
                               for thread in s.threads])
        self.numas = 4
        self.caches = 8
        self.nics = ['0000:c2:00.0', '0000:81:00.0', '0000:42:00.0',
                     '0000:01:00.0']
        self.nicsForSocket = [self.nics]
        self.nicForNuma = self.nics
        self.nicForCache = [self.nics[int(i/self.caches)]
                            for i in range(0,
                                           len(self.sockets) * self.caches)]
        self.nicForCore = [self.nics[int(i/self.coresPerSocket)]
                           for i in range(0, len(self.cores))]

        self.gpus = ['0000:c1:00.0', '0000:82:00.0', '0000:41:00.0',
                     '0000:03:00.0']
        self.gpusForSocket = [self.gpus]
        self.gpuForNuma = self.gpus

    def test_10_one_colocale(self):
        """
        Single locale gets all the GPUs
        """
        output = self.runCmd("./colocales -r 0 -n 1")
        self.assertIn("GPUS: " + ' '.join(self.gpus), output)

    def test_11_two_colocales(self):
        """
        Each co-locale gets the two GPUs closest to it
        """
        for i in range(0, 2):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -n 2" % i)
                x = i * 2
                self.assertIn("GPUS: " + ' '.join(self.gpus[x:x+1]), output)

    def test_12_four_colocales(self):
        """
        Each co-locale gets the GPU closest to it
        """
        for i in range(0, 4):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -n 4" % i)
                self.assertIn("GPUS: " + self.gpus[i], output)

    def test_13_oversubscribed(self):
        """
        All locales get all GPUs when oversubscribed.
        """
        for i in range(0, 4):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -a 4" % i)
                self.assertIn("GPUS: " + ' '.join(self.gpus), output)


class Hpc6aTests(TestCases.TestCase):
    """
    AWS hpc6a.48xlarge instance. Two sockets, two NUMA domains per socket, 48
    cores per socket, one thread per core, and one NIC not in a socket.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'hpc6a.48xlarge.xml')
        self.coresPerSocket = 48
        self.threadsPerCore = 1
        self.sockets = [Socket(self, i, 96) for i in range(0,2)]
        self.cores = sorted([core for s in self.sockets for core in s.cores])
        self.threads = sorted([thread for s in self.sockets
                               for thread in s.threads])
        self.numas = 2
        self.nics = ['0000:00:06.0']
        self.nicsForSocket = [['0000:00:06.0'], ['0000:00:06.0']]
        self.nicForNuma = ['0000:00:06.0', '0000:00:06.0', '0000:00:06.0',
                           '0000:00:06.0']


class M6inTests(TestCases.TestCase):
    """
    AWS m6in-dy-m6in32xlarge instance. Two sockets, one NUMA domain per
    socket, 32 cores per socket, two threads per core, and two NICs not in
    sockets.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'm6in-dy-m6in32xlarge.xml')
        self.coresPerSocket = 32
        self.threadsPerCore = 2
        self.sockets = [Socket(self, i, 64) for i in range(0,2)]
        self.cores = sorted([core for s in self.sockets for core in s.cores])
        self.threads = sorted([thread for s in self.sockets
                               for thread in s.threads])
        self.numas = 1
        self.nics= ['0000:00:06.0', '0000:00:08.0']

    def test_10_socket_unique_nics(self):
        """
        In this topology the NICs are not in sockets. Test that each locale
        gets a unique NIC.
        """
        nics = []
        for i in range(0, len(self.sockets)):
            with self.subTest(i=i):
                output = self.runCmd("./colocales -r %d -n %d -N %s" %
                                     (i, len(self.sockets), self.nics[0]))
                for line in output.split('\n'):
                    if line.startswith("NIC:"):
                        nic = line.split()[1]
                        self.assertIn(nic, self.nics)
                        self.assertNotIn(nic, nics)
                        nics.append(nic)
                        break

    def test_11_numa_unique_nics(self):
        """
        In this topology the NICs are not in NUMAs. Test that each locale
        gets a unique NIC.
        """
        numas = self.numas * len(self.sockets)
        nics = []
        for i in range(0, numas):
            output = self.runCmd("./colocales -r %d -n %d -N %s" %
                                 (i, numas, self.nics[0]))
            for line in output.split('\n'):
                if line.startswith("NIC:"):
                    nic = line.split()[1]
                    self.assertIn(nic, self.nics)
                    self.assertNotIn(nic, nics)
                    nics.append(nic)
                    break

class M1TestCases(object):
    """
    This outer class prevents the unittest framework from running the
    test cases in TestCase.
    """
    class M1Tests(TestCases.TestCase):
        """
        M1Pro/Max. One socket, one NUMA domain, three L2 caches. The first L2
        cache has two efficiency cores, the other two have four performance
        cores.
        """
        def setUp(self):
            super().setUp()
            self.coresPerSocket = 0
            self.threadsPerCore = 1
            s = Socket(self, 0, 0)
            s.cores = range(2, 2+8)
            s.threads = s.cores
            self.sockets = [s]
            self.cores = sorted([core for s in self.sockets for core in s.cores])
            self.threads = sorted([thread for s in self.sockets
                                for thread in s.threads])
            self.numas = 1
            self.nics= []

        def test_10_L2_binding(self):
            """
            Two co-locales should be bound to the two L2 caches that have performance
            cores.
            """
            for r in range(0, 1):
                output = self.runCmd("./colocales -r %d -n 2" % (r))
                start = 2 + r * 4
                cpus = stringify([start + j for j in range(0, 4)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)

        def test_11_no_binding(self):
            """
            Three, five, seven, and eight co-locales should be unbound
            """
            for (n, cores_per_locale, unused) in [(3, 2, 2), (5, 1, 3), (7, 1, 1), (8, 1, 0)]:
                for r in range(0, n-1):
                    output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                    start = 2 + r * cores_per_locale
                    cpus = stringify([start + j for j in range(0, cores_per_locale)])
                    self.assertIn("Physical CPUs: %s\n" % cpus, output)
                    if unused > 0:
                        self.assertIn("warning: %d cores are unused\n" % unused, output)

        def test_12_all_cores(self):
            """
            test various amounts of colocales with CHPL_RT_USE_PU_KIND=all
            """
            self.env['CHPL_RT_USE_PU_KIND'] = 'all'

            for (n, cores_per_locale, unused) in [(2, 5, 0), (3, 3, 1), (4, 2, 2), (5, 2, 0), (7, 1, 3), (8, 1, 2)]:
                for r in range(0, n-1):
                    output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                    start = r * cores_per_locale
                    cpus = stringify([start + j for j in range(0, cores_per_locale)])
                    self.assertIn("Physical CPUs: %s\n" % cpus, output)
                    if unused > 0:
                        self.assertIn("warning: %d cores are unused\n" % unused, output)

            self.env['CHPL_RT_USE_PU_KIND'] = 'none'

        def test_13_bind_to_llc(self):
            """
            Bind to the LLC with 3 colocales, should warn
            """
            self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "cache"

            output = self.runCmd("./colocales -r 0 -n 3")
            self.assertIn("warning: 2 cores are unused\n", output)
            self.assertIn("warning: Cannot bind to the architectural feature 'cache' with 3 co-locales. Falling back to no binding.\n", output)

            self.env['CHPL_RT_COLOCALE_OBJ_TYPE'] = 'none'


class M1ProTests(M1TestCases.M1Tests):
    """
    M1Pro. One socket, one NUMA domain, three L2 caches. The first L2
    cache has two efficiency cores, the other two have four performance
    cores.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'm1pro.xml')

class M1MaxTests(M1TestCases.M1Tests):
    """
    M1Max. One socket, one NUMA domain, three L2 caches. The first L2
    cache has two efficiency cores, the other two have four performance
    cores.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'm1max.xml')


class M3ProTests(TestCases.TestCase):
    """
    M1Pro/Max. One socket, one NUMA domain, three L2 caches. The first L2
    cache has two efficiency cores, the other two have four performance
    cores.
    """
    def setUp(self):
        super().setUp()
        setXML(self, 'm3pro.xml')
        self.coresPerSocket = 0
        self.threadsPerCore = 1
        s = Socket(self, 0, 0)
        s.cores = range(6, 6+6)
        s.threads = s.cores
        self.sockets = [s]
        self.cores = sorted([core for s in self.sockets for core in s.cores])
        self.threads = sorted([thread for s in self.sockets
                            for thread in s.threads])
        self.numas = 1
        self.nics= []

    def test_10_no_binding(self):
        """
        Two, Three, five, and size co-locales should be unbound
        """
        for (n, cores_per_locale, unused) in [(2, 3, 0), (3, 2, 0), (5, 1, 1), (6, 1, 0)]:
            for r in range(0, n-1):
                output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                start = 6 + r * cores_per_locale
                cpus = stringify([start + j for j in range(0, cores_per_locale)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)
                if unused > 0:
                    self.assertIn("warning: %d cores are unused\n" % unused, output)

    def test_11_all_cores(self):
        """
        test various amounts of colocales with CHPL_RT_USE_PU_KIND=all
        """
        self.env['CHPL_RT_USE_PU_KIND'] = 'all'

        for (n, cores_per_locale, unused) in [(2, 6, 0), (3, 4, 0), (4, 3, 0), (5, 2, 2), (6, 2, 0), (7, 1, 5), (12, 1, 0)]:
            for r in range(0, n-1):
                output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                start = r * cores_per_locale
                cpus = stringify([start + j for j in range(0, cores_per_locale)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)
                if unused > 0:
                    self.assertIn("warning: %d cores are unused\n" % unused, output)

        self.env['CHPL_RT_USE_PU_KIND'] = 'none'

    def test_12_bind_to_core(self):
        """
        Bind to the core with 3 colocales, should warn and fall back

        on M3Pro the with CHPL_RT_USE_PU_KIND=performance (the default),
        any kind of binding will always fail
        """
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "core"

        output = self.runCmd("./colocales -r 0 -n 3")
        self.assertIn("warning: Cannot bind to the architectural feature 'core' with 3 co-locales. Falling back to no binding.\n", output)

        self.env['CHPL_RT_COLOCALE_OBJ_TYPE'] = 'none'

class AlderLakeTests(TestCases.TestCase):
    """
    This alderlake config has 8 performance cores (2 hyperthreads) and 8 efficiency cores.
    """

    def setUp(self):
        super().setUp()
        setXML(self, '../cpuKinds/alderLake.xml')
        self.coresPerSocket = 0
        self.threadsPerCore = 2
        s = Socket(self, 0, 0)
        s.cores = range(0, 16, 2)
        s.threads = range(0, 16)
        self.sockets = [s]
        self.cores = sorted([core for s in self.sockets for core in s.cores])
        self.threads = sorted([thread for s in self.sockets
                            for thread in s.threads])
        self.numas = 1
        self.nics = []

    def test_10_no_binding(self):
        for n, cores_per_locale, unused in [
            (2, 4, 0),
            (3, 2, 2),
            (4, 2, 0),
            (5, 1, 3),
            (6, 1, 2),
            (7, 1, 1),
            (8, 1, 0)
        ]:
            for r in range(0, n-1):
                output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                start = r * cores_per_locale*2
                cpus = stringify([start + j for j in range(0, cores_per_locale*2, 2)])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)
                if unused > 0:
                    self.assertIn("warning: %d cores are unused\n" % unused, output)

    def test_11_all_cores(self):
        """
        test various amounts of colocales with CHPL_RT_USE_PU_KIND=all
        """
        self.env['CHPL_RT_USE_PU_KIND'] = 'all'

        all_cores = self.cores + [len(self.cores)*self.threadsPerCore+i for i in range(0, 8)]

        for n, cores_per_locale, unused in [
            (2, 8, 0),
            (3, 5, 1),
            (4, 4, 0),
            (5, 3, 1),
            (6, 2, 4),
            (7, 2, 2),
            (8, 2, 0),
            (9, 1, 7),
            (10, 1, 6),
            (11, 1, 5),
            (12, 1, 4),
            (13, 1, 3),
            (14, 1, 2),
            (15, 1, 1),
            (16, 1, 0)
        ]:
            for r in range(0, n-1):
                output = self.runCmd("./colocales -r {} -n {}".format(r, n))
                start = r * cores_per_locale
                cpus = stringify(all_cores[start:start + cores_per_locale])
                self.assertIn("Physical CPUs: %s\n" % cpus, output)
                if unused > 0:
                    self.assertIn("warning: %d cores are unused\n" % unused, output)

        self.env['CHPL_RT_USE_PU_KIND'] = 'none'

    def test_12_bind_to_core(self):
        """
        Bind to the core with 3 colocales, only use 3 cores, one per locale
        """
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "core"

        output = self.runCmd("./colocales -r 0 -n 3")
        # this is technically wrong, since we really only see 8 cores
        # but its good enough for now
        self.assertIn("warning: 13 cores are unused\n", output)
        self.assertIn("Physical CPUs: 0\n", output)

        output = self.runCmd("./colocales -r 1 -n 3")
        self.assertIn("warning: 13 cores are unused\n", output)
        self.assertIn("Physical CPUs: 2\n", output)

        output = self.runCmd("./colocales -r 2 -n 3")
        self.assertIn("warning: 13 cores are unused\n", output)
        self.assertIn("Physical CPUs: 4\n", output)

        self.env['CHPL_RT_COLOCALE_OBJ_TYPE'] = 'none'

    def test_13_bind_to_cache(self):
        """
        Bind to the cache with 3 colocales
        """
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "cache"

        output = self.runCmd("./colocales -r 0 -n 3")
        # TODO: what if I want to pin to the L2 cache?
        self.assertIn("error: Node only has 1 L3 cache(s)\n", output)

        self.env['CHPL_RT_COLOCALE_OBJ_TYPE'] = 'none'

class SuffixTests(TestFramework):
    """
    Tests functionality of "-nl" type suffix
    """
    def setUp(self):
        super().setUp()
        setupEx2(self)

    def test_00_socket_basic(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "socket"
        for i in range(0, len(self.sockets)):
            for n in self.nicsForSocket[i]:
                with self.subTest(i=i, n=n):
                    self.in_socket_test(i, n)

    def test_01_socket_too_many_colocales(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "socket"
        output = self.runCmd("./colocales -r 0 -n %d" % (len(self.sockets)+1))
        self.assertIn("error: Node only has %d socket(s)\n" %
                      len(self.sockets), output)

    def test_02_numa_basic(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "numa"
        for i in range(0, len(self.sockets) * self.numas):
            with self.subTest(i=i):
                self.in_numa_test(i, self.nicForNuma[i])

    def test_03_numa_too_many_colocales(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "numa"
        output = self.runCmd("./colocales -r 0 -n %d" %
                             (len(self.sockets)*self.numas+1))
        self.assertIn("error: Node only has %d NUMA domain(s)\n" %
                      (len(self.sockets) * self.numas), output)

    def test_04_cache_basic(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "cache"
        for i in range(0, len(self.sockets) * self.caches):
            with self.subTest(i=i):
                self.in_cache_test(i, self.nicForCache[i])

    def test_05_cache_too_many_colocales(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "cache"
        output = self.runCmd("./colocales -r 0 -n %d" %
                             (len(self.sockets)*self.caches+1))
        self.assertIn("error: Node only has %d L3 cache(s)\n" %
                      (len(self.sockets) * self.caches), output)

    def test_06_core_basic(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "core"
        for i in self.cores:
            with self.subTest(i=i):
                self.in_core_test(i, self.nicForCore[i])

    def test_07_core_too_many_colocales(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "core"
        output = self.runCmd("./colocales -r 0 -n %d" %
                             (len(self.cores) + 1))
        self.assertIn("error: Node only has %d core(s)\n" %
                      (len(self.cores)), output)

    def test_08_socket_unused_cores(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "socket"
        output = self.runCmd("./colocales -r 0 -n 1")
        self.assertIn("warning: %d cores are unused" % self.coresPerSocket,
                      output)

    def test_09_numa_unused_cores(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "numa"
        output = self.runCmd("./colocales -r 0 -n 1")
        self.assertIn("warning: %d cores are unused" %
                      ((self.coresPerSocket/self.numas * (self.numas - 1)) +
                       self.coresPerSocket), output)

    def test_10_cache_unused_cores(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "cache"
        output = self.runCmd("./colocales -r 0 -n 1")
        self.assertIn("warning: %d cores are unused" %
                      ((self.coresPerSocket/self.caches * (self.caches - 1)) +
                       self.coresPerSocket), output)

    def test_11_core_unused_cores(self):
        self.env["CHPL_RT_COLOCALE_OBJ_TYPE"] = "core"
        output = self.runCmd("./colocales -r 0 -n 1")
        self.assertIn("warning: %d cores are unused" % (len(self.cores) - 1),
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
