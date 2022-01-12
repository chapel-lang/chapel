import sys
import os

print(os.environ['CI_SITE_CONFIG'])
sys.path.append(os.environ['CI_SITE_CONFIG']) # for adding path for ci_site_config

import subprocess
import re
import ci_site_config
import common
import shlex
from abc import ABC, abstractmethod # abstract base class for creating abstract classes in python

job_cadence = os.environ['JOB_CADENCE']

# A Jenkins env variable for job name is composed of the name of the jenkins job and the branch name
# it is building for. for e.g. in our case jobname = 'ofi_libfabric/master'
class Test:
    def __init__ (self, jobname, buildno, testname, core_prov, fabric,
                  hosts, ofi_build_mode, util_prov=None):
        self.jobname = jobname
        self.buildno = buildno
        self.testname = testname
        self.core_prov = core_prov
        self.util_prov = "ofi_{}".format(util_prov) if util_prov != None else ""
        self.fabric = fabric
        self.hosts = hosts
        self.ofi_build_mode = ofi_build_mode
        self.job_cadence = job_cadence
        if (len(hosts) == 2):
            self.server = hosts[0]
            self.client = hosts[1]

        self.nw_interface = ci_site_config.interface_map[self.fabric]
        self.libfab_installpath = "{}/{}/{}/{}".format(ci_site_config.install_dir,
                                  self.jobname, self.buildno, self.ofi_build_mode)

        self.env = [("FI_VERBS_MR_CACHE_ENABLE", "1"),\
                    ("FI_VERBS_INLINE_SIZE", "256")] \
                    if self.core_prov == "verbs" else []
class FiInfoTest(Test):
    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 hosts, ofi_build_mode, util_prov=None):

        super().__init__(jobname, buildno, testname, core_prov, fabric,
                     hosts, ofi_build_mode, util_prov)

        self.fi_info_testpath =  "{}/bin".format(self.libfab_installpath)

    @property
    def cmd(self):
        return "{}/fi_info ".format(self.fi_info_testpath)

    @property
    def options(self):
        if (self.util_prov):
            opts  = "-f -p {};{}".format(self.core_prov, self.util_prov)
        else:
            opts = "-f -p {}".format(self.core_prov)

        return opts

    def execute_cmd(self):
        command = self.cmd + self.options
        outputcmd = shlex.split(command)
        common.run_command(outputcmd)


class Fabtest(Test):

    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 hosts, ofi_build_mode, util_prov=None):

        super().__init__(jobname, buildno, testname, core_prov, fabric,
                         hosts, ofi_build_mode, util_prov)
        self.fabtestpath = "{}/bin".format(self.libfab_installpath)
        self.fabtestconfigpath = "{}/share/fabtests".format(self.libfab_installpath)
    def get_exclude_file(self):
        path = self.libfab_installpath
        efile_path = "{}/share/fabtests/test_configs".format(path)

        prov = self.util_prov if self.util_prov else self.core_prov
        efile_old = "{path}/{prov}/{prov}.exclude".format(path=efile_path,
                      prov=prov)

        if self.util_prov:
            efile = "{path}/{util_prov}/{core_prov}/exclude".format(path=efile_path,
                      util_prov=self.util_prov, core_prov=self.core_prov)
        else:
            efile = "{path}/{prov}/exclude".format(path=efile_path,
                      prov=self.core_prov)

        if os.path.isfile(efile):
            return efile
        elif os.path.isfile(efile_old):
            return efile_old
        else:
            print("Exclude file: {} not found!".format(efile))
            return None

    @property
    def cmd(self):
        return "{}/runfabtests.sh ".format(self.fabtestpath)

    @property
    def options(self):
        opts = "-T 300 -vvv -p {} -S ".format(self.fabtestpath)
        if (self.core_prov == "verbs" and self.nw_interface):
            opts = "{} -s {} ".format(opts, common.get_node_name(self.server,
                    self.nw_interface)) # include common.py
            opts = "{} -c {} ".format(opts, common.get_node_name(self.client,
                    self.nw_interface)) # from common.py

        if (self.core_prov == "shm"):
            opts = "{} -s {} ".format(opts, self.server)
            opts = "{} -c {} ".format(opts, self.client)
            opts += "-N "

        if not re.match(".*sockets|udp.*", self.core_prov):
            opts = "{} -t all ".format(opts)

        efile = self.get_exclude_file()
        if efile:
            opts = "{} -R ".format(opts)
            opts = "{} -f {} ".format(opts, efile)

        for key,val in self.env:
            opts = "{options} -E {key}={value} ".format(options = opts,
                    key=key, value=val)

        if self.util_prov:
            opts = "{options} {core};{util} ".format(options=opts,
                    core=self.core_prov, util=self.util_prov)
        else:
            opts = "{options} {core} ".format(options=opts,
                    core=self.core_prov)

        if (self.core_prov == "shm"):
            opts += "{} {} ".format(self.server, self.server)
        else:
            opts += "{} {} ".format(self.server, self.client)

        return opts

    @property
    def execute_condn(self):
        return True if (self.core_prov != 'shm' or \
                        self.ofi_build_mode == 'dbg') else False

    def execute_cmd(self):
        curdir = os.getcwd()
        os.chdir(self.fabtestconfigpath)
        command = self.cmd + self.options
        outputcmd = shlex.split(command)
        common.run_command(outputcmd)
        os.chdir(curdir)

class ShmemTest(Test):
    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 hosts, ofi_build_mode, util_prov=None):

        super().__init__(jobname, buildno, testname, core_prov, fabric,
                         hosts, ofi_build_mode, util_prov)

        #self.n - number of hosts * number of processes per host
        self.n = 4
        # self.ppn - number of processes per node.
        self.ppn = 2
        self.shmem_dir = "{}/shmem".format(self.libfab_installpath)

    @property
    def cmd(self):
        #todo: rename mpi_testpath to testpath to make it generic for shmem and mpitest
        return "{}/run_shmem.sh ".format(ci_site_config.mpi_testpath)

    def options(self, shmem_testname):

        if self.util_prov:
            prov = "{core};{util} ".format(core=self.core_prov,
                    util=self.util_prov)
        else:
            prov = self.core_prov

        opts = "-n {n} -hosts {server},{client} -shmem_dir={shmemdir} \
                -libfabric_path={path}/lib -prov '{provider}' -test {test} \
                -server {server} -inf {inf}" \
                .format(n=self.n, server=self.server, client=self.client, \
                shmemdir=self.shmem_dir, path=self.libfab_installpath, \
                provider=prov, test=shmem_testname, \
                inf=ci_site_config.interface_map[self.fabric])
        return opts

    @property
    def execute_condn(self):
        return True if (self.job_cadence == 'daily' and \
                        (self.core_prov == "psm2" or \
                        self.core_prov == "sockets")) \
                    else False

    def execute_cmd(self, shmem_testname):
        command = self.cmd + self.options(shmem_testname)
        outputcmd = shlex.split(command)
        common.run_command(outputcmd)


class MpiTests(Test):
    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 mpitype, hosts, ofi_build_mode, util_prov=None):

        super().__init__(jobname, buildno, testname, core_prov,
                         fabric, hosts, ofi_build_mode, util_prov)
        self.mpi = mpitype

    @property
    def cmd(self):
        if (self.mpi == "impi" or self.mpi == "mpich"):
            self.testpath = ci_site_config.mpi_testpath
            return "{}/run_{}.sh ".format(self.testpath,self.mpi)
        elif(self.mpi =="ompi"):
            self.testpath = "{}/ompi/bin".format(self.libfab_installpath)
            return "{}/mpirun ".format(self.testpath)

    @property
    def options(self):
        opts = []
        if (self.mpi == "impi" or self.mpi == "mpich"):
            opts = "-n {} -ppn {} -hosts {},{} ".format(self.n,self.ppn,
                    self.server,self.client)

            if (self.mpi == "impi"):
                opts = "{} -mpi_root={} ".format(opts,
                        ci_site_config.impi_root)
            else:
                opts = "{} -mpi_root={}/mpich".format(opts,
                        self.libfab_installpath)

            opts = "{} -libfabric_path={}/lib ".format(opts,
                    self.libfab_installpath)

            if self.util_prov:
                opts = "{options} -prov {core};{util} ".format(options=opts,
                        core=self.core_prov, util=self.util_prov)
            else:
                opts = "{} -prov {} ".format(opts, self.core_prov)

            for key, val in self.env:
                opts = "{} -genv {} {} ".format(opts, key, val)

        elif (self.mpi == "ompi"):
            opts = "-np {} ".format(self.n)
            hosts = ",".join([":".join([host,str(self.ppn)]) \
                    for host in self.hosts])

            opts = "{} --host {} ".format(opts, hosts)

            if self.util_prov:
                opts = "{} --mca mtl_ofi_provider_include {};{} ".format(opts,
                        self.core_prov,self.util_prov)
            else:
                opts = "{} --mca mtl_ofi_provider_include {} ".format(opts,
                        self.core_prov)

            opts += "--mca orte_base_help_aggregate 0 "
            opts += "--mca mtl ofi --mca pml cm -tag-output "
            for key,val in self.env:
                opts = "{} -x {}={} ".format(opts,key,val)
        return opts

    @property
    def mpi_gen_execute_condn(self):
        #Skip MPI tests for udp, verbs(core) providers.
        # we would still have MPI tests runnning for
        # verbs-rxd and verbs-rxm providers
        return True if (self.core_prov != "udp" and \
                        self.core_prov != "shm" and \
                       (self.core_prov != "verbs" or \
                       self.util_prov == "ofi_rxm" or \
                       self.util_prov == "ofi_rxd")) else False

# IMBtests serves as an abstract class for different
# types of intel MPI benchmarks. Currently we have
# the mpi1 and rma tests enabled which are encapsulated
# in the IMB_mpi1 and IMB_rma classes below.

class IMBtests(ABC):
    """
    This is an abstract class for IMB tests.
    currently IMB-MPI1 and IMB-RMA tests are
    supported. In future there could be more.
    All abstract  methods must be implemented.
    """

    @property
    @abstractmethod
    def imb_cmd(self):
        pass

    @property
    @abstractmethod
    def execute_condn(self):
        pass

class IMBmpi1(IMBtests):

    def __init__(self):
        self.additional_tests = [
                                   "Biband",
                                   "Uniband",
                                   "PingPongAnySource",
                                   "PingPingAnySource",
                                   "PingPongSpecificSource",
                                   "PingPingSpecificSource"
        ]

    @property
    def imb_cmd(self):
        return "{}/intel64/bin/IMB-MPI1 -include {}".format(ci_site_config.impi_root, \
                ','.join(self.additional_tests))

    @property
    def execute_condn(self):
        return True

class IMBrma(IMBtests):
    def __init__(self, core_prov):
        self.core_prov =  core_prov

    @property
    def imb_cmd(self):
        return "{}/intel64/bin/IMB-RMA".format(ci_site_config.impi_root)

    @property
    def execute_condn(self):
        return True if (self.core_prov != "verbs") else False

# MpiTestIMB class inherits from the MPITests class.
# It uses the same options method and class variables as all MPI tests.
# It creates IMB_xxx test objects for each kind of IMB test.
class MpiTestIMB(MpiTests):

    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 mpitype, hosts, ofi_build_mode, util_prov=None):
        super().__init__(jobname, buildno, testname, core_prov, fabric,
                         mpitype, hosts, ofi_build_mode, util_prov)

        self.n = 4
        self.ppn = 1
        self.mpi1 = IMBmpi1()
        self.rma = IMBrma(self.core_prov)

    @property
    def execute_condn(self):
        return True if (self.mpi == "impi") else False

    def execute_cmd(self):
        command = self.cmd + self.options
        if(self.mpi1.execute_condn):
            outputcmd = shlex.split(command +  self.mpi1.imb_cmd)
            common.run_command(outputcmd)
        if (self.rma.execute_condn):
            outputcmd = shlex.split(command + self.rma.imb_cmd)
            common.run_command(outputcmd)

class MpichTestSuite(MpiTests):

    def __init__(self, jobname, buildno, testname, core_prov, fabric,
		     mpitype, hosts, ofi_build_mode, util_prov=None):
            super().__init__(jobname, buildno, testname, core_prov, fabric,
			     mpitype,  hosts, ofi_build_mode, util_prov)
            self.mpichsuitepath =  "{}/{}/mpichsuite/test/mpi/" \
                                   .format(self.libfab_installpath, self.mpi)
            self.pwd = os.getcwd()

    def testgroup(self, testgroupname):

        testpath = "{}/{}".format(self.mpichsuitepath, testgroupname)
        tests = []
        with open("{}/testlist".format(testpath)) as file:
            for line in file:
                if(line[0] != '#' and  line[0] != '\n'):
                    tests.append((line.rstrip('\n')).split(' '))

        return tests

    def options(self, nprocs, timeout=None):
        if (self.mpi == "impi" or self.mpi == "mpich"):
            if (self.mpi == "impi"):
                mpiroot = ci_site_config.impi_root
            else:
                mpiroot = "{}/mpich".format(self.libfab_installpath)
            if (self.util_prov):
                prov = "\"{};{}\"".format(self.core_prov, self.util_prov)
            else:
                prov = self.core_prov

            if (timeout != None):
                os.environ['MPIEXEC_TIMEOUT']=timeout

            opts = "-n {np} -hosts {s},{c} -mpi_root={mpiroot} \
                    -libfabric_path={installpath}/lib -prov {provider} "  \
                    .format(np=nprocs, s=self.server, c=self.client, \
                            provider=prov, mpiroot=mpiroot, \
                            installpath=self.libfab_installpath)

        elif (self.mpi == "ompi"):
            print(self.mpi)

        return opts

    @property
    def execute_condn(self):
        return True if (self.mpi == 'impi' and  self.core_prov != 'psm2' \
                        and self.core_prov != 'sockets') else False

    def execute_cmd(self, testgroupname):
        print("Running Tests: " + testgroupname)
        tests = []
        time = None
        os.chdir("{}/{}".format(self.mpichsuitepath,testgroupname))
        tests = self.testgroup(testgroupname)
        for test in tests:
            testname = test[0]
            nprocs = test[1]
            args = test[2:]
            for item in args:
               itemlist =  item.split('=')
               if (itemlist[0] == 'timelimit'):
                   time = itemlist[1]
            opts = self.options(nprocs, timeout=time)
            testcmd = self.cmd + opts +"./{}".format(testname)
            outputcmd = shlex.split(testcmd)
            common.run_command(outputcmd)
        os.chdir(self.pwd)


class MpiTestOSU(MpiTests):

    def __init__(self, jobname, buildno, testname, core_prov, fabric,
                 mpitype, hosts, ofi_build_mode, util_prov=None):
        super().__init__(jobname, buildno, testname, core_prov, fabric,
                         mpitype, hosts, ofi_build_mode, util_prov)

        self.n = 4
        self.ppn = 2
        self.two_proc_tests = {'osu_latency',
                               'osu_bibw',
                               'osu_latency_mt',
                               'osu_bw','osu_get_latency',
                               'osu_fop_latency',
                               'osu_acc_latency',
                               'osu_get_bw',
                               'osu_put_latency',
                               'osu_put_bw',
                               'osu_put_bibw',
                               'osu_cas_latency',
                               'osu_get_acc_latency'
                              }

        self.osu_mpi_path = "{}/{}/osu/libexec/osu-micro-benchmarks/mpi/". \
                            format(self.libfab_installpath,mpitype)

    @property
    def execute_condn(self):
        # sockets and psm2 have some issues with OSU benchmark testing.
        return True if ((self.job_cadence  == 'daily') and \
                        (self.mpi != "ompi" or \
                        (self.core_prov != "sockets" and \
                         self.core_prov != "psm2" and \
                         self.ofi_build_mode!="dbg"))) \
                    else False

    def execute_cmd(self):
        assert(self.osu_mpi_path)
        p = re.compile('osu_put*')
        for root, dirs, tests in os.walk(self.osu_mpi_path):
            for test in tests:
                if test in self.two_proc_tests:
                    self.n=2
                    self.ppn=1
                else:
                    self.n=4
                    self.ppn=2
                # for sockets provider skip 'osu_put' benchmark tests as they fail.
                if(self.core_prov !='sockets' or p.search(test)== None):
                    launcher = self.cmd + self.options
                    osu_cmd = os.path.join(root, test)
                    command = launcher + osu_cmd
                    outputcmd = shlex.split(command)
                    common.run_command(outputcmd)


