#!/usr/bin/env python3

#
# Copyright 2023-2024 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import chapel
import chapel
import chapel.replace
import sys
import argparse
from driver import LintDriver
from rules import register_rules
from lsp import run_lsp

def print_violation(node, name):
    location = node.location()
    first_line, _ = location.start()
    print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

def main():
    parser = argparse.ArgumentParser( prog='chplcheck', description='A linter for the Chapel language')
    parser.add_argument('filenames', nargs='*')
    parser.add_argument('--disable-rule', action='append', dest='disabled_rules', default=[])
    parser.add_argument('--enable-rule', action='append', dest='enabled_rules', default=[])
    parser.add_argument('--lsp', action='store_true', default=False)
    parser.add_argument('--skip-unstable', action='store_true', default=False)
    parser.add_argument('--internal-prefix', action='append', dest='internal_prefixes', default=[])
    args = parser.parse_args()

    driver = LintDriver(skip_unstable = args.skip_unstable, internal_prefixes = args.internal_prefixes)
    # register rules before enabling/disabling
    register_rules(driver)
    driver.disable_rules(*args.disabled_rules)
    driver.enable_rules(*args.enabled_rules)

    if args.lsp:
        run_lsp(driver)
        return

    printed_warning = False

    for (filename, context) in chapel.files_with_contexts(args.filenames):
        context.set_module_paths([], [])

        # Silence errors, warnings etc. -- we're just linting.
        with context.track_errors() as errors:
            asts = context.parse(filename)
            violations = list(driver.run_checks(context, asts))
            #sort the failures in order of appearance
            violations.sort(key=lambda f : f[0].location().start()[0])
            for (node, rule) in violations:
                print_violation(node, rule)
                printed_warning = True

    if printed_warning:
        sys.exit(1)

if __name__ == "__main__":
    main()
