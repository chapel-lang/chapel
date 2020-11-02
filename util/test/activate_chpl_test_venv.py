#!/usr/bin/env python3

import os
import sys

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_home_utils
import chpl_make

def error(message):
    sys.stdout.write('[Warning: {0}]\n'.format(message))
    raise ImportError(message)

def log(message):
    sys.stdout.write('[{0}]\n'.format(message))

# Activate a venv that has testing infrastructure requirements installed
def activate_venv():
    venv_dir = chpl_home_utils.get_chpl_test_venv()
    default_venv_dir = chpl_home_utils.get_chpl_venv()

    # user asserts that system already has the required dependencies installed:
    if venv_dir == 'none':
        log('Skipping venv activation because CHPL_TEST_VENV_DIR={0}. '
            'test-venv requirements must be available.'.format(venv_dir))

    else:
        # using custom venv, does not check that our test requirements are met
        if venv_dir != default_venv_dir:
            log('Using custom venv because CHPL_TEST_VENV_DIR={0}. '
                'test-venv requirements must be available'.format(venv_dir))

        # check Chapel test-venv for successful installation sentinel
        else:
            sentinel_file = os.path.join(venv_dir, 'chpl-test-reqs')
            if not os.path.isfile(sentinel_file):
                error('Chapel test venv not available, run a top-level '
                      '`{0} test-venv`'.format(chpl_make.get()))

        # "activate" the venv
        # this is based upon activate_this.py from virtualenv.
        old_os_path = os.environ.get('PATH', '')

        # do not change PATH so that the system python may be used.
        # python scripts using the virtualenv dependencies should use
        #
        #   from test import activate_chpl_test_venv
        #
        # near to the 'import' of a virtual dependency.

        #os.environ['PATH'] = os.path.dirname(os.path.abspath(__file__)) + os.pathsep + old_os_path

        base = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        site_packages = os.path.join(base, 'lib', 'python%s' % sys.version[:3], 'site-packages')

        prev_sys_path = list(sys.path)
        import site
        site.addsitedir(site_packages)
        sys.real_prefix = sys.prefix
        sys.prefix = base
        # Move the added items to the front of the path:
        new_sys_path = []
        for item in list(sys.path):
            if item not in prev_sys_path:
                new_sys_path.append(item)
                sys.path.remove(item)
        sys.path[:0] = new_sys_path

activate_venv()
