#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Ensure filenames for test-*.bash scripts match the config name registered
inside them.
"""

from __future__ import print_function

import os.path
import re
import sys

for line in sys.stdin.readlines():
    filename, content = line.split(':', 1)

    filename_parts = os.path.splitext(filename)
    filename_base = filename_parts[0]

    pattern = re.compile(r'CHPL_NIGHTLY_TEST_CONFIG_NAME="(?P<config_name>[a-z0-9\-.]+)"',
                         re.IGNORECASE)
    match = pattern.search(content)
    config_name = None
    if match is not None:
        config_name = match.group('config_name')
    else:
        print('[ERROR] Could not find nightly test config name '
              'in: {0}'.format(filename))
        sys.exit(0)

    if not filename_base.endswith(config_name):
        print('[ERROR] test script name: "{0}" does not match its config name: "{1}"'.format(
            filename, config_name))
