#!/usr/bin/env python

from __future__ import print_function

import os
import sys

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import utils
import chpl_platform
import chpl_make

# Activate a virtualenv that has testing infrastructure requirements installed
#
# By default, we will try to use $CHPL_HOME/third-party/chpl-venv as our
# virtualenv. We then check for a sentinel file that test-venv creates when
# it's been successfully installed and finally activate the virtualenv.
#
# A user can also set CHPL_TEST_VENV_DIR to specify the path to a custom
# virtualenv that will be activated. "none" is a special value that means skip
# activating a virtualenv. If CHPL_TEST_VENV_DIR is set, a user is asserting
# that their virtualenv or local install has the requirements available.
#
# Note that this method does not allow us to specify python versions to use and
# instead will use the system default. Long term we probably want to have a
# wrapper script that activates the virtualenv and just calls start_test.

def error(message):
    print('[Error: {0}]'.format(message))
    exit(1)

def activate_venv():

    custom_venv_dir_var = 'CHPL_TEST_VENV_DIR'
    custom_venv_dir = os.getenv(custom_venv_dir_var, '').strip()

    # user asserts that system already has the required dependencies installed:
    if custom_venv_dir == 'none':
        print('[Skipping virtualenv activation because {0}={1}. test-venv '
              'requirements must be available.]'.format(custom_venv_dir_var,
              custom_venv_dir))

    else:
        venv_dir = None

        # using custom venv, does not check that our test requirements are met
        if custom_venv_dir:
            venv_dir = custom_venv_dir
            print('[Using custom  virtualenv because {0}={1}. test-venv '
                  'requirements must be available]'.format(custom_venv_dir_var,
                  custom_venv_dir))

        # check Chapel test-venv for successful installation sentinel
        else:
            chpl_home = os.path.join(utils.get_chpl_home(), '')
            third_party = os.path.join(chpl_home, 'third-party')
            target_platform = chpl_platform.get('target')

            venv_dir = os.path.join(third_party, 'chpl-venv', 'install',
                                    target_platform, 'chpl-virtualenv')
            sentinel_file = os.path.join(venv_dir, 'chpl-test-reqs')
            if not os.path.isfile(sentinel_file):
                error('Chapel test virtualenv is not available, run `{0} '
                      'test-venv` from {1}'.format(chpl_make.get(), chpl_home))

        activation_file = os.path.join(venv_dir, 'bin', 'activate_this.py')
        if not os.path.isfile(activation_file):
            error('Activation file {0} is missing'.format(activation_file))

        # actually activate
        execfile(activation_file, dict(__file__=activation_file))


activate_venv()
