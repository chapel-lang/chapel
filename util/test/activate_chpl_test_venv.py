#!/usr/bin/env python

from __future__ import print_function

import os
import sys

chplenv_dir = os.path.join(os.path.dirname(__file__), '..')
sys.path.insert(0, os.path.abspath(chplenv_dir))

from chplenv import chpl_home_utils
from chplenv import chpl_make

# Activate a virtualenv that has testing infrastructure requirements installed
#
# By default, we will try to use
#   $CHPL_HOME/third-party/chpl-venv/install/$CHPL_HOST_PLATFORM/$python_version_dir/chpl-virtualenv
# as our virtualenv. We then check for a sentinel file that test-venv
# creates when it's been successfully installed and finally activate
# the virtualenv.
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

    venv_dir = chpl_home_utils.get_chpl_test_venv()
    default_venv_dir = chpl_home_utils.get_chpl_venv()

    # user asserts that system already has the required dependencies installed:
    if venv_dir == 'none':
        print('[Skipping virtualenv activation because venv_dir={0}. test-venv '
              'requirements must be available.]'.format(venv_dir))

    else:
        # using custom venv, does not check that our test requirements are met
        if venv_dir != default_venv_dir:
            print('[Using custom virtualenv because venv_dir={1}. test-venv '
                  'requirements must be available]'.format(venv_dir))

        # check Chapel test-venv for successful installation sentinel
        else:
            sentinel_file = os.path.join(venv_dir, 'chpl-test-reqs')
            if not os.path.isfile(sentinel_file):
                error('Chapel test virtualenv not available, run a top-level '
                      '`{0} test-venv`'.format(chpl_make.get()))

        activation_file = os.path.join(venv_dir, 'bin', 'activate_this.py')
        if not os.path.isfile(activation_file):
            error('Activation file {0} is missing'.format(activation_file))

        # actually activate
        execfile(activation_file, dict(__file__=activation_file))


activate_venv()
