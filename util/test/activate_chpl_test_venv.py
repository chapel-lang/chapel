#!/usr/bin/env python

import os
import sys

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_home_utils
import chpl_make

def error(message):
    sys.stdout.write('[Error: {0}]\n'.format(message))
    exit(1)

def log(message):
    sys.stdout.write('[{0}]\n'.format(message))

# Activate a virtualenv that has testing infrastructure requirements installed
def activate_venv():
    venv_dir = chpl_home_utils.get_chpl_test_venv()
    default_venv_dir = chpl_home_utils.get_chpl_venv()

    # user asserts that system already has the required dependencies installed:
    if venv_dir == 'none':
        log('Skipping virtualenv activation because CHPL_TEST_VENV_DIR={0}. '
            'test-venv requirements must be available.'.format(venv_dir))

    else:
        # using custom venv, does not check that our test requirements are met
        if venv_dir != default_venv_dir:
            log('Using custom virtualenv because CHPL_TEST_VENV_DIR={0}. '
                'test-venv requirements must be available'.format(venv_dir))

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
        with open(activation_file) as f:
            code = compile(f.read(), activation_file, 'exec')
            exec(code, dict(__file__=activation_file))


activate_venv()
