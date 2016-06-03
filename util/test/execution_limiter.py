from __future__ import with_statement

import getpass
import os
import sys
import tempfile
import time

def log_info(msg):
    if os.getenv('CHPL_TEST_DEBUG_RUNNING_EXECUTABLE_LOCK') is not None:
        sys.stdout.write('[Exec-lock Info: {0}]\n'.format(msg))
        sys.stdout.flush()

# Empty class that can be used with a context manager. Does not
# limit how many executables can run at once.
class NoLock():
    def __enter__(self):
        pass
    def __exit__(self, type, value, traceback):
        pass

# Class that uses a file as a mutex so that only one executable can
# run per user, per machine. This is useful when you want to
# oversubscribe paratest, but don't want to cause too many
# executables to run at once since some tests can consume the whole
# machine which can starve other executables, causing timeouts.
#
# This works by using a file as a lock/mutex. It's race-y so it's
# possible (though relatively unlikely) that two or more executables
# could run at the same time. That's a little unfortunate, but not a
# huge deal as it's pretty unlikely we'll get multiple resource
# heavy tests running simultaneously
#
# The constructor takes the name of the executable and its timeout
# value.
class FileLock():
    exec_name = ''
    exec_timeout = ''
    lock_file = ''

    # simple backoff sleep, so that we have relatively fast latency for a bunch
    # of quick running tests, but don't poll the machine continuously if a test
    # is taking a while to run. see #3928 for why the times/backoff were chosen
    sleep_time = .02
    def _backoff_sleep(self, max_sleep=.5, backoff_factor=1.3):
	time.sleep(self.sleep_time)
	if self.sleep_time < max_sleep:
	    self.sleep_time *= backoff_factor
   
    def __init__(self, exec_name, exec_timeout):
	lock_name = '{0}-chpl_program_executing'.format(getpass.getuser())
        self.lock_file = os.path.join(tempfile.gettempdir(), lock_name)
        self.exec_name = exec_name
        self.exec_timeout = exec_timeout

    def __enter__(self):
        self._lock()

    def __exit__(self, type, value, traceback):
        self._unlock()

    # parse the lock_file and return how long ago the lock_file was
    # modified, the name of the test that has the lock, and the
    # test's timeout value. If there's issues opening or parsing the
    # file, return a default of: (1, "corrupted_lock_file", 0), so
    # that the waiting test can continue.
    def _parse_lock_file(self):
        default_lock_file = (1, "corrupted_lock_file", 0)
        try:
            with open(self.lock_file, 'r') as f:
                lines = f.read().splitlines()
                other_filename = lines[0]
                other_timeout = int(lines[1])

                modified = time.time() - os.path.getmtime(self.lock_file)
                return (modified, other_filename, other_timeout)
        except:
            log_info('Failed to parse lock file, using default one')
            return default_lock_file

    # Grab the execution "lock", spin waiting while some other test
    # has the lock. If for some reason the other test didn't clean
    # up the locking file, we'll continue if the lock hasn't been
    # modified in the other test's timeout value.
    def _lock(self):
	modified = 0
	other_timeout = 1
	while os.path.exists(self.lock_file) and modified < other_timeout:
            modified, other_filename, other_timeout = self._parse_lock_file()
            log_info('"{0}" is waiting at most {1}s for "{2}" (started {3:.1f}s '
                     'ago) to finish executing'.format(
                     self.exec_name, other_timeout, other_filename, modified))
            self._backoff_sleep()

	# actually grab the "lock" by creating the file
        log_info('Grabbing exec-lock for "{0}"'.format(self.exec_name))
	with open(self.lock_file, 'w') as f:
	    f.write('{0}\n{1}\n'.format(self.exec_name, self.exec_timeout))


    def _unlock(self):
	try:
	    # release the "lock" by removing the file
            log_info('Releasing exec-lock for "{0}"'.format(self.exec_name))
	    os.remove(self.lock_file)
	except:
	    pass
