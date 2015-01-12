#!/usr/bin/env python

"""Verify the setchplenv.* script correctly set the environment for using chpl,
et al.

Also verify there are the correct setchplenv.* scripts and error if new ones
show up without updating this test.
"""

from __future__ import print_function

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

import chpl_platform


class SetChplEnvTests(unittest.TestCase):

    known_shells = ['bash', 'csh', 'fish', 'sh']

    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '../..'))
    chpl_home = os.environ.get('CHPL_HOME', repo_root)
    chpl_platform = chpl_platform.get()

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
        out, _ = proc.communicate(input=cmd)
        if proc.returncode != 0:
            raise ValueError('Non-zero exit code ({0}) from: {1}'.format(
                proc.returncode, cmd))
        return out

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
            prefix = '{source_cmd} {setchplenv_script} ;\n'.format(
                source_cmd=source_cmd, setchplenv_script=setchplenv_script)
            if post_source_cmd is not None:
                prefix = '{0}{1} ;\n'.format(prefix, post_source_cmd)
            return '{0}{1}'.format(prefix, cmd)

        path_out = self.run_cmd(shell_cmd, get_cmd('echo $PATH'))
        path = path_out.splitlines()[-1]

        path_parts = path.split(path_sep)
        self.assertTrue(len(path_parts) >= 2)

        path_chpl = path_parts[0].replace(self.chpl_home, 'CHPL_HOME').replace(
            self.chpl_platform, 'CHPL_PLATFORM')
        path_util = path_parts[1].replace(self.chpl_home, 'CHPL_HOME')

        self.assertEqual('CHPL_HOME/bin/CHPL_PLATFORM', path_chpl)
        self.assertEqual('CHPL_HOME/util', path_util)

        manpath_out = self.run_cmd(shell_cmd, get_cmd('echo $MANPATH'))
        manpath = manpath_out.splitlines()[-1]

        manpath_parts = manpath.split(path_sep)
        self.assertTrue(len(manpath_parts) >= 1)

        manpath_chpl = manpath_parts[0].replace(self.chpl_home, 'CHPL_HOME')
        self.assertEqual('CHPL_HOME/man', manpath_chpl)

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

    def test_setchplenv__fish(self):
        """Verify fish versions of setchplenv.* work as expected."""
        self.check_scripts('fish', '.', ' ')

    def test_setchplenv__sh(self):
        """Verify sh versions of setchplenv.* work as expected."""
        self.check_scripts('sh', '.', ':')

    def test_setchplenv__zsh(self):
        """Verify bash versions of setchplenv.* work as expected with zsh."""
        self.check_scripts('bash', 'source', ':', shell_cmd='zsh')


if __name__ == '__main__':
    unittest.main()
