#!/usr/bin/env python

"""Verify the setchplenv.* script correctly set the environment for using chpl,
et al.

Also verify there are the correct setchplenv.* scripts and error if new ones
show up without updating this test.
"""

from __future__ import print_function

import distutils.spawn
import glob
import os
import os.path
import shlex
import subprocess
import sys
import unittest

# Add the chplenv dir to the python path.
chplenv_dir = os.path.join(os.path.dirname(__file__), '../../util/chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_bin_subdir


def _skip_if(condition, reason):
    """Wrapper around unittest.skipIf, if available. If not, do not run test
    when condition is True. unittest.skipIf was not added until python
    2.7. Chapel testing requires python 2.6 support.
    """
    if hasattr(unittest, 'skipIf'):
        return unittest.skipIf(condition, reason)
    elif condition:
        # Skip the test by instead running a noop function that just print a
        # message.
        def skip_it(func):
            def noop(*args, **kwargs):
                print('Skipping "{0}" because: {1}'.format(func.__name__, reason))
            return noop
        return skip_it
    else:
        # Run the test by simply returning the original test function.
        def pass_thru(func):
            return func
        return pass_thru


class SetChplEnvTests(unittest.TestCase):

    known_shells = ['bash', 'csh', 'fish', 'sh']

    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '../..'))
    chpl_home = os.environ.get('CHPL_HOME', repo_root)
    bin_subdir = chpl_bin_subdir.get()

    util_dir = os.path.join(chpl_home, 'util')
    quickstart_dir = os.path.join(util_dir, 'quickstart')

    @classmethod
    def setchplenv_script(cls, shell):
        return 'setchplenv.{0}'.format(shell)

    @classmethod
    def util_script(cls, shell):
        return os.path.join(cls.util_dir, cls.setchplenv_script(shell))

    @classmethod
    def quickstart_script(cls, shell):
        return os.path.join(cls.quickstart_dir, cls.setchplenv_script(shell))

    def run_cmd(self, shell, cmd):
        """Run command in shell and return output."""
        proc = subprocess.Popen(
            [shell],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            cwd=self.chpl_home,
            env=os.environ.copy()
        )
        out, _ = proc.communicate(input=str.encode(cmd))
        if proc.returncode != 0:
            raise ValueError('Non-zero exit code ({0}) from: {1}'.format(
                proc.returncode, cmd))
        return out.decode()

    def test_known_shells(self):
        """Verify all known shells have equivalent setchplenv.* scripts in util/ and
        util/quickstart/.
        """
        for shell in self.known_shells:
            self.assertTrue(os.path.exists(self.util_script(shell)))
            self.assertTrue(os.path.exists(self.quickstart_script(shell)))

    def test_for_new_scripts(self):
        """Verify new scripts are tested here."""
        util_scripts = glob.glob(self.util_script('*'))
        quickstart_scripts = glob.glob(self.quickstart_script('*'))
        self.assertEqual(len(self.known_shells), len(util_scripts),
                         'Untested setchplenv.* script in util/')
        self.assertEqual(len(self.known_shells), len(quickstart_scripts),
                         'Untested setchplenv.* script in util/quickstart/')
        for shell in self.known_shells:
            self.assertTrue(hasattr(self, 'test_setchplenv__{0}'.format(shell)),
                            'No test case for shell: {0}'.format(shell))

    def check_shell(self, shell, setchplenv_script, source_cmd, path_sep,
                    post_source_cmd=None, shell_cmd=None):
        """Run checks for a single shell."""
        if shell_cmd is None:
            shell_cmd = shell

        def get_cmd(cmd):
            prefix = '{source_cmd} {setchplenv_script}\n'.format(
                source_cmd=source_cmd, setchplenv_script=setchplenv_script)
            if post_source_cmd is not None:
                prefix = '{0}{1}\n'.format(prefix, post_source_cmd)
            return '{0}{1}\n'.format(prefix, cmd)

        def get_var(var_name):
            """Get and return a var from setchplenv.* env."""
            out = self.run_cmd(shell_cmd, get_cmd('echo ${0}'.format(var_name)))
            value = out.splitlines()[-1]
            return value

        def get_path_var(var_name):
            """Gets the path-like var and returns a list of its parts."""
            path = get_var(var_name)
            path_parts = path.split(path_sep)
            return path_parts

        path_parts = get_path_var('PATH')
        self.assertTrue(len(path_parts) >= 2)

        expected_path_chpl = os.path.join(self.chpl_home, 'bin', self.bin_subdir)
        expected_path_util = os.path.join(self.chpl_home, 'util')

        actual_path_chpl = path_parts[0]
        actual_path_util = path_parts[1]

        self.assertEqual(os.stat(expected_path_chpl), os.stat(actual_path_chpl))
        self.assertEqual(os.stat(expected_path_util), os.stat(actual_path_util))

        manpath_parts = get_path_var('MANPATH')
        self.assertTrue(len(manpath_parts) >= 1)

        expected_manpath_chpl = os.path.join(self.chpl_home, 'man')
        manpath_chpl = manpath_parts[0]
        self.assertEqual(os.stat(expected_manpath_chpl), os.stat(manpath_chpl))

        actual_chpl_home = get_var('CHPL_HOME')
        self.assertEqual(os.stat(self.chpl_home), os.stat(actual_chpl_home))

        if 'quickstart' in os.path.relpath(setchplenv_script, self.util_dir):
            self.assertEqual('none', get_var('CHPL_COMM'))
            self.assertEqual('fifo', get_var('CHPL_TASKS'))
            self.assertEqual('none', get_var('CHPL_GMP'))
            self.assertEqual('none', get_var('CHPL_REGEXP'))
            self.assertEqual('none', get_var('CHPL_LLVM'))
            self.assertEqual('cstdlib', get_var('CHPL_MEM'))

    def check_scripts(self, shell, source_cmd, path_sep,
                      post_source_cmd=None, shell_cmd=None):
        """Run check_shell on util/ and quickstart/ scripts."""
        self.check_shell(shell, self.util_script(shell), source_cmd, path_sep,
                         post_source_cmd=post_source_cmd, shell_cmd=shell_cmd)
        self.check_shell(shell, self.quickstart_script(shell), source_cmd, path_sep,
                         post_source_cmd=post_source_cmd, shell_cmd=shell_cmd)

    def test_setchplenv__bash(self):
        """Verify bash versions of setchplenv.* work as expected."""
        self.check_scripts('bash', 'source', ':')

    def test_setchplenv__csh(self):
        """Verify csh versions of setchplenv.* work as expected."""
        self.check_scripts('csh', 'source', ':', post_source_cmd='rehash', shell_cmd='tcsh')

    @_skip_if(distutils.spawn.find_executable('fish') is None,
              'fish is not installed on system.')
    def test_setchplenv__fish(self):
        """Verify fish versions of setchplenv.* work as expected."""
        self.check_scripts('fish', '.', ' ')

    def test_setchplenv__sh(self):
        """Verify sh versions of setchplenv.* work as expected."""
        self.check_scripts('sh', '.', ':')

    @_skip_if(distutils.spawn.find_executable('zsh') is None,
              'zsh is not installed on system.')
    def test_setchplenv__zsh(self):
        """Verify bash versions of setchplenv.* work as expected with zsh."""
        self.check_scripts('bash', 'source', ':', shell_cmd='zsh')


if __name__ == '__main__':
    unittest.main()
