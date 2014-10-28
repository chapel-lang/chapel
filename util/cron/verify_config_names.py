#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Ensure filenames for test-*.bash scripts match the config name registered
inside them.
"""

from __future__ import print_function

import sys

for line in sys.stdin.readlines():
    filename, content = line.split(':', 1)

    config_name = content.split('"')[1]
    expected_script_name = 'test-{0}.bash'.format(config_name)

    if not filename.endswith(expected_script_name):
        print('[ERROR] test script name: "{0}" does not match its config name: "{1}"'.format(
            filename, config_name))
