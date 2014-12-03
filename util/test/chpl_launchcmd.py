#!/usr/bin/env python

"""Run Chapel test (execution only) inside pbs or slurm batch job.

The job name is set from the environment variable CHPL_LAUNCHCMD_NAME_PREFIX
(defaulting to Chpl) and the name of the program being executing. For example,
running `chpl_launchcmd.py ./hello` would use the name Chpl-hello.

The high level overview of what this does:

 * Detect slurm or flavor of qsub (either PBSPro or moab).
   * If none, raises error.
 * Parses number locales and wall time from the test command args so they can
   be sent to qsub/slurm.
 * Rebuilds the test command.
 * Launches the job by passing the test command on stdin to qsub (batch
   mode). Slurm jobs just run the chapel executable, setting
   CHPL_LAUNCHER_USE_SBATCH=true. Stdout/stderr are directed to a temporary
   file designated by the script.
 * Polls qstat/squeue with the given job id every second until the status is
   complete.
 * Prints the contents of the temp file with stdout/stderr from the job to
   stdout.
 * Cleans up the temp file and exits.

"""

from __future__ import print_function, unicode_literals, with_statement

import argparse
import contextlib
import datetime
import logging
import os
import os.path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import time
import xml.etree.ElementTree

__all__ = ('main')


def main():
    """Run the program!"""
    job = AbstractJob.init_from_environment()
    print(job.run(), end='')


class AbstractJob(object):
    """Abstract job runner implementation."""

    # These class attributes should always be None on the AbstractJob
    # class. They *should only* be defined on and accessed from a sub class.

    # submit_bin is the program used to submit jobs (i.e. qsub).
    submit_bin = None

    # status_bin is the program used to query the status of jobs (i.e. qstat,
    # squeue)
    status_bin = None

    # argument name to use when specifying specific nodes (i.e. hostlist,
    # mppnodes)
    hostlist_resource = None

    # argument name for specifying number of nodes (i.e. nodes, mppwidth)
    num_nodes_resource = None

    # arugment name for specifying number of processing elements per node (i.e. mppnppn)
    processing_elems_per_node_resource = None

    # argument name for specifying number of cpus (i.e. mppdepth)
    num_cpus_resource = None

    def __init__(self, test_command, reservation_args):
        """Initialize new job runner.

        :type test_command: list
        :arg test_command: command to run in qsub

        :type reservation_args: argparse.Namespace
        :arg reservation_args: reservation arguments parsed from cli
        """
        self.test_command = test_command
        self.num_locales = reservation_args.numLocales
        self.walltime = reservation_args.walltime
        self.hostlist = reservation_args.hostlist

        logging.debug('Created instance of: {0}'.format(self))

    def __repr__(self):
        """Return string representation of this instance."""
        cls_name = str(type(self))
        attrs = ', '.join(map(lambda x: '{0}={1}'.format(x, getattr(self, x, None)),
                              ['test_command', 'num_locales', 'walltime', 'hostlist']))
        return '{0}({1})'.format(cls_name, attrs)

    @property
    def full_test_command(self):
        """Returns instance's test_command prefixed with command to change to
        testing_dir. This is required to support both PBSPro and moab flavors
        of PBS. Whereas moab provides a -d argument when calling qsub, both
        support the $PBS_O_WORKDIR argument.

        :rtype: list
        :returns: command to run in qsub with changedir call
        """
        full_test_command = ['cd', '$PBS_O_WORKDIR', '&&']
        full_test_command.extend(self.test_command)
        return full_test_command

    @property
    def num_cpus(self):
        """Returns the number of cpus that qsub should reserve. PBSPro requires
        the cpu reservation be given to both qsub, and aprun.

        If cnselect is not callable, raise RuntimeError.

        :rtype: int
        :returns: Number of cpus to reserve, or -1 if there was no cnselect output
        """
        try:
            logging.debug('Checking for number of cpus to reserve.')
            cnselect_proc = subprocess.Popen(
                ['cnselect', '-Lnumcores'],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)

            logging.debug('Communicating with cnselect process.')
            stdout, stderr = cnselect_proc.communicate()
        except OSError as ex:
            raise RuntimeError(ex)
        first_line = stdout.split('\n')[0]
        if first_line:
            return int(first_line)
        else:
            msg = 'cnselect -Lnumcores had no output.'
            logging.error(msg)
            raise ValueError(msg)

    @property
    def job_name(self):
        """Returns job name string from test command and CHPL_LAUNCHCMD_NAME_PREFIX
        env var.

        :rtype: str
        :returns: job name
        """
        prefix = os.environ.get('CHPL_LAUNCHCMD_NAME_PREFIX', 'Chpl')
        logging.debug('Job name prefix is: {0}'.format(prefix))

        cmd_basename = os.path.basename(self.test_command[0])
        logging.debug('Test command basname: {0}'.format(cmd_basename))

        job_name = '{0}-{1}'.format(prefix, cmd_basename)
        logging.info('Job name is: {0}'.format(job_name))
        return job_name

    def run(self):
        """Run batch job in subprocess and wait for job to complete. When finished,
        returns output as string.

        :rtype: str
        :returns: stdout/stderr from job
        """
        with _temp_dir() as working_dir:
            output_file = os.path.join(working_dir, 'test_output.log')
            testing_dir = os.getcwd()

            job_id = self.submit_job(testing_dir, output_file)
            logging.info('Test has been queued (job id: {0}). Waiting for output...'.format(job_id))

            # TODO: The while condition here should look for jobs that become held,
            #       are in the queue too long, or ??? and do something
            #       intelligent. For example, if the job is in the queue longer
            #       than the walltime, it should probably be deleted (qdel
            #       <job_id>) and a timeout should be reported. Here are all the
            #       pbs (torque) job statuses:
            #
            #           C -     Job is completed after having run/
            #           E -  Job is exiting after having run.
            #           H -  Job is held.
            #           Q -  job is queued, eligible to run or routed.
            #           R -  job is running.
            #           T -  job is being moved to new location.
            #           W -  job is waiting for its execution time
            #                (-a option) to be reached.
            #           S -  (Unicos only) job is suspend.
            #
            #       (thomasvandoren, 2014-04-09)

            def job_status(job_id, output_file):
                """Returns the status of the job specified by job_id

                 The status is determined by calling status(job_id). If that
                 call is successful the result is returned. The exact code
                 returned is up to status(job_id) but it must support 'C' for
                 complete, 'Q' for queued/waiting to run, and 'R' for running 

                 status(job_id) can raise a ValueError, which can indicate that
                 the job has completed *and* been dequeued. If the output file
                 exists and the job has been dequeued, it is safe to assume it
                 completed. Otherwise we raise the error
                """
                try:
                    job_status = self.status(job_id)
                    return job_status
                except ValueError as ex:
                    # ValueError may indicate that the job completed and was
                    # dequeued before we last checked the status. If the output
                    # file exists, assume success. Otherwise re raise error
                    # message.
                    if os.path.exists(output_file):
                        return 'C'
                    raise

            exec_start_time = time.time()
            alreadyRunning = False
            status = job_status(job_id, output_file)
            while status != 'C':
                if not alreadyRunning and status == 'R':
                    alreadyRunning = True
                    exec_start_time = time.time()
                status = job_status(job_id, output_file)
                time.sleep(.5)

            exec_time = time.time() - exec_start_time
            # Note that this time isn't very accurate as we don't get the exact
            # start or end time, however this does give a better estimate than
            # timing the whole binary for cases where the time in the queue is
            # large. It tends to be a second or two larger than real exec time
            exec_time_file = os.environ.get('CHPL_LAUNCHCMD_EXEC_TIME_FILE')
            if exec_time_file != None:
                with open(exec_time_file, 'w') as fp:
                    fp.write('{0:3f}'.format(exec_time))

            logging.debug('{0} reports job {1} as complete.'.format(
                self.status_bin, job_id))

            if not os.path.exists(output_file):
                logging.error('Output file from job does not exist at: {0}'.format(
                    output_file))
                raise ValueError('[Error: output file from job (id: {0}) does not exist at: {1}]'.format(
                    job_id, output_file))

            logging.debug('Reading output file.')
            with open(output_file, 'r') as fp:
                output = fp.read()
            logging.info('The test finished with output of length {0}.'.format(len(output)))

        return output

    def submit_job(self, testing_dir, output_file):
        """Submit a new job using ``testing_dir`` as the working dir and
        ``output_file`` as the location for the output. Returns the job id on
        success. AbstractJob does not implement this method. It is the
        responsibility of the sub class.

        :type testing_dir: str
        :arg testing_dir: working directory for running test

        :type output_file: str
        :arg output_file: output log filename

        :rtype: str
        :returns: job id
        """
        raise NotImplementedError('submit_job class method is implemented by sub classes.')

    @classmethod
    def _detect_job_flavor(cls):
        """Returns appropriate class based on the detected version of pbs or slurm in
        the environment.

        If neither srun or qsub is not callable, raise RuntimeError.

        If MOABHOMEDIR is set in the environment, assume moab and return
        MoabJob type.

        Otherwise, if qsub is callable assume PBSPro, and return PbsProJob
        type.

        If srun is callable, assume slurm, and return SlurmJob.

        :rtype: type
        :returns: SlurmJob, MoabJob, or PbsProJob depending on environment
        """
        qsub_callable = False
        qsub_version = ''
        srun_callable = False
        srun_version = ''

        def get_output(cmd):
            proc = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT
            )
            logging.debug('Communicating with job process.')
            stdout, stderr = proc.communicate()
            return stdout

        # Detect if qsub is callable, and capture version output.
        try:
            qsub_version = get_output(['qsub', '--version'])
            qsub_callable = True
        except OSError:
            pass

        # Detect if srun is callable, and capture version output.
        try:
            srun_version = get_output(['srun', '--version'])
            srun_callable = True
        except OSError:
            pass

        # Favor slurm, since Cray version of slurm comes with qsub command
        # that is wrapper around slurm apis.
        if srun_callable:
            return SlurmJob
        elif qsub_callable and os.environ.has_key('MOABHOMEDIR'):
            return MoabJob
        elif qsub_callable:
            return PbsProJob
        else:  # not (qsub_callable or srun_callable)
            raise RuntimeError('Could not find PBS or SLURM on system.')

    def _launch_qsub(self, testing_dir, output_file):
        """Launch job using qsub and return job id. Raises RuntimeError if
        self.submit_bin is anything but qsub.

        :type testing_dir: str
        :arg testing_dir: working directory for running test

        :type output_file: str
        :arg output_file: output log filename

        :rtype: str
        :returns: job id
        """
        if self.submit_bin != 'qsub':
            raise RuntimeError('_launch_qsub called for non-pbs job type!')

        logging.info(
            'Starting {0} job "{1}" on {2} nodes with walltime {3} '
            'and output file: {4}'.format(
                self.submit_bin, self.job_name, self.num_locales,
                self.walltime, output_file))

        # TODO: create self._qsub_command property. (thomasvandoren, 2014-07-23)
        submit_command = [self.submit_bin, '-V', '-N', self.job_name, '-j', 'oe',
                          '-o', output_file]
        if self.num_locales >= 0:
            submit_command.append('-l')
            submit_command.append('{0}={1}'.format(
                self.num_nodes_resource, self.num_locales))
        if self.walltime is not None:
            submit_command.append('-l')
            submit_command.append('walltime={0}'.format(self.walltime))
        if self.hostlist is not None:
            submit_command.append('-l')
            submit_command.append('{0}={1}'.format(
                self.hostlist_resource, self.hostlist))
        if self.num_cpus_resource is not None:
            submit_command.append('-l')
            submit_command.append('{0}={1}'.format(
                self.num_cpus_resource, self.num_cpus))
        if self.processing_elems_per_node_resource is not None:
            submit_command.append('-l')
            submit_command.append('{0}={1}'.format(
                self.processing_elems_per_node_resource, 1))

        logging.debug('submit command to run: {0}'.format(submit_command))

        logging.debug('Opening {0} subprocess.'.format(self.submit_bin))
        submit_proc = subprocess.Popen(
            submit_command,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd=testing_dir,
            env=os.environ.copy()
        )

        test_command_str = ' '.join(self.full_test_command)
        logging.debug('Communicating with {0} subprocess. Sending test command on stdin: {1}'.format(
            self.submit_bin, test_command_str))
        stdout, stderr = submit_proc.communicate(input=test_command_str)
        logging.debug('{0} process returned with status {1}, stdout: {2} stderr: {3}'.format(
            self.submit_bin, submit_proc.returncode, stdout, stderr))

        if submit_proc.returncode != 0:
            msg = '{0} failed with exit code {1} and output: {2}'.format(
                self.submit_bin, submit_proc.returncode, stdout)
            logging.error(msg)
            raise ValueError(msg)

        job_id = stdout.strip()
        return job_id

    @classmethod
    def init_from_environment(cls):
        """Factory to initialize new job runner instance based on version of
        pbs available and command line arguments.

        :rtype: AbstractJob
        :returns: subclass of AbstractJob based on environment
        """
        args, unparsed_args = cls._parse_args()
        cls._setup_logging(args.verbose)

        logging.info('Num locales is: {0}'.format(args.numLocales))
        logging.info('Walltime is set to: {0}'.format(args.walltime))

        test_command = cls._get_test_command(args, unparsed_args)
        logging.debug('Test command is: {0}'.format(' '.join(test_command)))
        if not test_command:
            logging.error('No test command provided.')
            raise ValueError('No test command found.')

        job_flavor = cls._detect_job_flavor()
        logging.info('Detected job flavor: {0}'.format(job_flavor.__name__))
        return job_flavor(test_command, args)

    @classmethod
    def status(cls, job_id):
        """Query job stat using ``status_bin``. AbstractJob does not implement this
        method. It is the responsibility of the sub class.

        :type job_id: str
        :arg job_id: job id

        :rtype: str
        :returns: job status
        """
        raise NotImplementedError('status class method is implemented by sub classes.')

    @classmethod
    def _cli_walltime(cls, walltime_str):
        """Returns walltime_str if it can be parsed by one of the known walltime
        formats. Raises ValueError if walltime_str does not match a known format.

        :type walltime_str: str
        :arg walltime_str: walltime string from command line

        :rtype: str
        :returns: valid walltime string from command line
        """
        try:
            seconds = int(walltime_str)
            logging.debug('Parsed walltime as integer seconds: {0}'.format(seconds))
            return walltime_str
        except ValueError:
            pass

        try:
            seconds = float(walltime_str)
            logging.debug('Parsed walltime as float seconds: {0}'.format(seconds))
            return walltime_str
        except ValueError:
            pass

        # http://www.csc.fi/english/pages/louhi_guide/batch_jobs/commands/qsub
        known_formats = [
            '%M:%S',
            '%H:%M:%S',
            '%M:%S.%f',
            '%H:%M:%S.%f',
        ]
        for fmt in known_formats:
            try:
                walltime = datetime.datetime.strptime(walltime_str, fmt)
                logging.debug('Parsed walltime as datetime with format {0}: {1}'.format(
                    fmt, walltime))
                return walltime_str
            except ValueError:
                pass

        raise ValueError('Did not recognize walltime: {0}'.format(walltime_str))

    @classmethod
    def _get_test_command(cls, args, unparsed_args):
        """Returns test command by folding numLocales args into unparsed command line
        args.

        :type args: argparse.Namespace
        :arg args: Namespace from parsing original args

        :type unparsed_args: list
        :arg unparsed_args: list of unparsed command line args that make up test command

        :rtype: list
        :returns: command to be tested in qsub
        """
        logging.debug('Rebuilding test command from parsed args: {0} and '
                      'unparsed args: {1}'.format(args, unparsed_args))
        if args.numLocales >= 0:
            unparsed_args.append('-nl')
            unparsed_args.append(str(args.numLocales))
        logging.debug('Rebuild test command: {0}'.format(unparsed_args))
        return unparsed_args

    @classmethod
    def _parse_args(cls):
        """Parse and return command line arguments. Returns tuple of Namespace with
        parsed args and unparsed args.
        """
        class OurFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
            pass

        parser = argparse.ArgumentParser(
            description=__doc__,
            formatter_class=OurFormatter)
        parser.add_argument('-v', '--verbose', action='store_true',
                            help='Verbose output.')
        parser.add_argument('-nl', '--numLocales', type=int, default=-1,
                            help='Number locales.')
        parser.add_argument('--n', help='Placeholder')
        parser.add_argument('--walltime', type=cls._cli_walltime,
                            help='Timeout as walltime for qsub.')
        parser.add_argument('--hostlist',
                            help=('Optional hostlist specification for reserving '
                                  'specific nodes. Can also be set with env var '
                                  'CHPL_LAUNCHCMD_HOSTLIST'))

        args, unparsed_args = parser.parse_known_args()

        # Allow hostlist to be set in environment variable CHPL_LAUNCHCMD_HOSTLIST.
        if args.hostlist is None:
            args.hostlist = os.environ.get('CHPL_LAUNCHCMD_HOSTLIST')

        # It is bad form to use a two character argument with only a single
        # dash. Unfortunately, we support it. And unfortunately, python argparse
        # thinks --n is the same thing. So, we pull out --n above so we can put it
        # back in the unparsed args here.
        if args.n:
            logging.debug('Found a --n arg. Putting it back in the unparsed args.')
            unparsed_args.append('--n={0}'.format(args.n))

        return args, unparsed_args

    @classmethod
    def _qstat(cls, job_id, args=None):
        """Call qstat and return output from stdout.

        Raises ValueError if exit code is non-zero.

        :type job_id: str
        :arg job_id: pbs job id

        :type args: list
        :arg args: additional arguments to pass qstat

        :rtype: str
        :returns: qsub job status
        """
        if args is None:
            args = []

        qstat_command = ['qstat'] + args + [job_id]
        logging.debug('qstat command to run: {0}'.format(qstat_command))

        logging.debug('Opening qstat subprocess.')
        qstat_proc = subprocess.Popen(
            qstat_command,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            env=os.environ.copy()
        )

        logging.debug('Communicating with qstat subprocess.')
        stdout, stderr = qstat_proc.communicate()
        logging.debug('qstat process returned with status {0}, stdout: {1}, and stderr: {2}'.format(
            qstat_proc.returncode, stdout, stderr))

        if qstat_proc.returncode != 0:
            raise ValueError('Non-zero exit code {0} from qstat: "{1}"'.format(
                qstat_proc.returncode, stdout))
        else:
            return stdout

    @classmethod
    def _setup_logging(cls, verbose=False):
        """Setup logging to console.

        :type verbose: bool
        :arg verbose: if True, set log level to DEBUG
        """
        # logging module configures default handlers when logging.debug/info/etc
        # are called. In order for our basicConfig call to work, we need to get rid
        # of those. This is generally a bad practice unless we are absolutely sure
        # we are the top level script and we won't break other logging. That's
        # probably true here.
        #
        # See note here: https://docs.python.org/2/library/logging.html#logging.log
        logging.root.handlers = []

        if verbose:
            log_level = logging.DEBUG
        else:
            log_level = logging.WARN
        logging.basicConfig(
            level=log_level, format='[%(module)s] %(asctime)s [%(levelname)s] %(msg)s')
        logging.debug('Verbose logging enabled.')


class MoabJob(AbstractJob):
    """Moab implementation of pbs job runner."""

    submit_bin = 'qsub'
    status_bin = 'qstat'
    hostlist_resource = 'hostlist'
    num_nodes_resource = 'nodes'
    processing_elems_per_node_resource = None
    num_cpus_resource = None

    @classmethod
    def status(cls, job_id):
        """Query job status using qstat.

        :type job_id: str
        :arg job_id: pbs job id

        :rtype: str
        :returns: qsub job status
        """
        output = cls._qstat(job_id, args=['-x'])
        try:
            root = xml.etree.ElementTree.fromstring(output)
            return root.find('Job').find('job_state').text
        except AttributeError as ex:
            logging.exception('Could not find job_state element in xml output: {0}'.format(ex))
            logging.error('XML output: {0}'.format(output))
            raise
        except Exception as ex:
            logging.exception('Failed to parse qstat output: {0}'.format(ex))
            logging.error('XML output: {0}'.format(output))
            raise

    def submit_job(self, testing_dir, output_file):
        """Launch job using qsub and return job id.

        :type testing_dir: str
        :arg testing_dir: working directory for running test

        :type output_file: str
        :arg output_file: output log filename

        :rtype: str
        :returns: job id
        """
        return self._launch_qsub(testing_dir, output_file)


class PbsProJob(AbstractJob):
    """PBSPro implementation of pbs job runner."""

    submit_bin = 'qsub'
    status_bin = 'qstat'
    hostlist_resource = 'mppnodes'
    num_nodes_resource = 'mppwidth'
    processing_elems_per_node_resource = 'mppnppn'

    # If CHPL_PBSPRO_NO_MPPDEPTH is set in the environment, set class attribute
    # to None. Otherwise, default to mppdepth.
    #
    # This allows callers to optionally disable this particular setting, which
    # can conflict with the hostlist/mppnodes setting.
    num_cpus_resource = 'mppdepth' if 'CHPL_PBSPRO_NO_MPPDEPTH' not in os.environ else None

    @property
    def job_name(self):
        """Takes the job_name from the super class, AbstractJob, and returns
        the last 15 characters. PBSPro limits job name to 15 characters.

        :rtype: str
        :returns: pbs job name
        """
        super_name = super(PbsProJob, self).job_name
        job_name = super_name[-15:]
        logging.info('PBSPro job name is: {0}'.format(job_name))
        return job_name

    @classmethod
    def status(cls, job_id):
        """Query job status using qstat.

        Assumes ``qstat <job_id>`` output is of the form:

        ::

            Job id            Name             User              Time Use S Queue
            ----------------  ---------------- ----------------  -------- - -----
            1889416.sdb       lj               tvandoren         00:00:03 R workq

        :type job_id: str
        :arg job_id: pbs job id

        :rtype: str
        :returns: qsub job status
        """
        output = cls._qstat(job_id)
        lines = output.splitlines()

        if len(lines) != 3:
            logging.error('Unexpected number of lines in qstat output: {0}'.format(output))
            raise ValueError('Expected 3 lines of qstat output, not {0}.'.format(len(output)))

        header_line = lines[0]
        job_line = lines[-1]

        # Use regex to find position of status. Then extract the one character
        # status from the job line.
        pattern = re.compile('\sS\s')
        match = pattern.search(header_line)
        if match is not None:
            status_char = match.start() + 1
            return job_line[status_char]
        else:
            logging.error('Could not find S column in header line of qstat output.')
            raise ValueError('Could not find {0} pattern in header line: {1}'.format(
                pattern.pattern, header_line))

    def submit_job(self, testing_dir, output_file):
        """Launch job using qsub and return job id.

        :type testing_dir: str
        :arg testing_dir: working directory for running test

        :type output_file: str
        :arg output_file: output log filename

        :rtype: str
        :returns: job id
        """
        return self._launch_qsub(testing_dir, output_file)


class SlurmJob(AbstractJob):
    """SLURM implementation of abstract job runner."""

    submit_bin = None
    status_bin = 'squeue'
    hostlist_resource = 'nodelist'
    num_nodes_resource = None
    processing_elems_per_node_resource = None
    num_cpus_resource = None

    @classmethod
    def status(cls, job_id):
        """Query job status using squeue.

        :type job_id: str
        :arg job_id: squeue job id

        :rtype: str
        :returns: squeue job status
        """
        squeue_command = [
            'squeue',
            '--noheader',
            '--format', '%A %T',  # "<job_id> <status>"
            '--states', 'all',
            '--job', job_id,
        ]
        logging.debug('squeue command to run: {0}'.format(squeue_command))

        logging.debug('Opening squeue subprocess.')
        squeue_proc = subprocess.Popen(
            squeue_command,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            env=os.environ.copy()
        )

        logging.debug('Communicating with squeue subprocess.')
        stdout, stderr = squeue_proc.communicate()
        logging.debug('squeue process returned with status {0}, stdout: {1}, stderr: {2}'.format(
            squeue_proc.returncode, stdout, stderr))

        if squeue_proc.returncode != 0:
            raise ValueError('Non-zero exit code {0} from squeue: "{1}"'.format(
                squeue_proc.returncode, stdout))

        failure_statuses = ['CANCELLED', 'FAILED', 'TIMEOUT',
                            'BOOT_FAIL', 'NODE_FAIL', 'PREEMPTED']

        queued_statuses = ['CONFIGURING', 'PENDING']

        status_parts = stdout.split(' ')
        if len(status_parts) == 2:
            status = status_parts[1].strip()
            logging.info('Status for job {0} is: {1}'.format(job_id, status))

            if status == 'COMPLETED':
                logging.info('Job finished with status: {0}'.format(status))
                return 'C'
            elif status in failure_statuses:
                logging.info('Job finished with status: {0}'.format(status))
                return 'C'
            elif status in queued_statuses:
                return 'Q'
            else:
                return 'R'  # running
        else:
            raise ValueError('Could not parse output from squeue: {0}'.format(stdout))

    def submit_job(self, testing_dir, output_file):
        """Launch job using executable. Set CHPL_LAUNCHER_USE_SBATCH=true in
        environment to avoid using expect script. The executable will create a
        sbatch script and submit it. Parse and return the job id after job is
        submitted.

        :type testing_dir: str
        :arg testing_dir: working directory for running test

        :type output_file: str
        :arg output_file: output log filename

        :rtype: str
        :returns: job id
        """
        env = os.environ.copy()
        env['CHPL_LAUNCHER_USE_SBATCH'] = 'true'
        env['CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME'] = output_file

        cmd = self.test_command[:]
        # Add --nodelist into the command line
        if self.hostlist is not None:
            cmd.append('--{0}={1}'.format(
                self.hostlist_resource, self.hostlist))

        # Add --walltime back into the command line.
        if self.walltime is not None:
            cmd.append('--walltime')
            cmd.append(self.walltime)

        logging.debug('Command to submit job: {0}'.format(cmd))

        logging.debug('Opening job subprocess')
        submit_proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd=testing_dir,
            env=env
        )

        logging.debug('Communicating with job subprocess')
        stdout, stderr = submit_proc.communicate()
        logging.debug('Job process returned with status {0}, stdout: {1}, stderr: {2}'.format(
            submit_proc.returncode, stdout, stderr))

        if submit_proc.returncode != 0:
            msg = 'Job submission ({0}) failed with exit code {1} and output: {2}'.format(
                cmd, submit_proc.returncode, stdout)
            logging.error(msg)
            raise ValueError(msg)

        # Output is: Submitted batch job 106001
        id_parts = stdout.split(' ')
        if len(id_parts) < 4:
            raise ValueError('Could not parse output from sbatch submission: {0}'.format(stdout))
        else:
            job_id = id_parts[3].strip()
            return job_id


@contextlib.contextmanager
def _temp_dir(dir_prefix='chapel-test-tmp'):
    """Context manager that creates a temporary directory in the current working
    directory with name of dir_prefix. When the manager exits, the directory is
    deleted.

    :type dir_prefix: str
    :arg dir_prefix: temp dir name prefix
    """
    try:
        cwd = os.getcwd()
        logging.debug('Creating temporary working directory in: {0}'.format(cwd))
        tmp_dir = tempfile.mkdtemp(prefix=dir_prefix, dir=cwd)

        logging.debug('Yielding temporary directory context manager.')
        yield tmp_dir
    finally:
        logging.debug('Deleting temporary working directory at: {0}'.format(tmp_dir))
        shutil.rmtree(tmp_dir)


if __name__ == '__main__':
    main()
