""" Execution limiter implementations to help limit the number of concurrently
running chpl executables """

import getpass
import os
import tempfile
import py3_compat

try:
    import activate_chpl_test_venv
    import filelock
except ImportError:
    pass

class NoLock():
    """ Empty class that can be used with a context manager. Does not limit how
        many executables can run at once. """
    def __enter__(self):
        pass
    def __exit__(self, exc_type, exc_value, traceback):
        pass


class FileLock():
    """ Simple wrapper over filelock that will only let one executable run per
        user, per machine.

        filelock is a platform independent lock that uses 1 of 3 platform
        dependent implementations. It will use `fcntl.flock` on unix systems
        (including mac and cygwin) to create a hard lock, `msvcrt.locking` on
        windows systems, and a soft file-based lock otherwise. fcntl and msvcrt
        don't have issues with 'stale' locks because the lock is held on the
        file descriptor and gets dropped by the OS even if the program crashes.
        The soft file-based lock can cause stale locks, so if we ever run into
        a platform where fcntl/msvcrt aren't supported we should re-add our own
        implementation that can deal with stale lock files.

        Also note that fcntl/msvcrt will leave the actual file used for locking
        around. Removing the file would "unlock" the lock, so there'd be a race
        between unlocking/removing and locking (we'd effectively have a double
        unlock). This is a common problem for file-based locks including
        "fasteners" (https://github.com/harlowja/fasteners/issues/26) """
    lock_file = ''
    lock = None

    def __init__(self):
        lock_name = '{0}-chpl_program_executing'.format(getpass.getuser())
        lock_dir = os.getenv('CHPL_TEST_LIMIT_RUNNING_EXECUTABLES_DIR', tempfile.gettempdir())
        py3_compat.makedirs(lock_dir, exist_ok=True)

        self.lock_file = os.path.join(lock_dir, lock_name)
        self.lock = filelock.FileLock(self.lock_file)

    def __enter__(self):
        self.lock.acquire()

    def __exit__(self, exc_type, exc_value, traceback):
        self.lock.release()
