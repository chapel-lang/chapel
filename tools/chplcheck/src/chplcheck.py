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

import os
import sys
import importlib.util

def print_violation(node, name):
    location = node.location()
    first_line, _ = location.start()
    print("{}:{}: node violates rule {}".format(location.path(), first_line, name))

def load_module(driver: LintDriver, file_path: str):
    """
    Load a module from a file path.
    This is used to extend the linter with custom rules.
    """
    module_name = os.path.basename(file_path).removesuffix('.py')

    spec = importlib.util.spec_from_file_location(module_name, file_path)
    if spec is None:
        raise ValueError(f"Could not load module from {file_path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    if spec.loader is None:
        raise ValueError(f"Could not load module from {file_path}")
    spec.loader.exec_module(module)
    rule_func_name = "rules"
    rule_func = getattr(module, rule_func_name, None)
    if rule_func is None or not callable(rule_func):
        raise ValueError(f"Could not find rule function '{rule_func_name}' in {file_path}")
    rule_func(driver)

def main():
    parser = argparse.ArgumentParser( prog='chplcheck', description='A linter for the Chapel language')
    parser.add_argument('filenames', nargs='*')
    parser.add_argument('--disable-rule', action='append', dest='disabled_rules', default=[])
    parser.add_argument('--enable-rule', action='append', dest='enabled_rules', default=[])
    parser.add_argument('--lsp', action='store_true', default=False)
    parser.add_argument('--skip-unstable', action='store_true', default=False)
    parser.add_argument('--internal-prefix', action='append', dest='internal_prefixes', default=[])
    parser.add_argument("--add-rules", action='append', default=[], help="Add a custom rule file")
    args = parser.parse_args()

    driver = LintDriver(skip_unstable = args.skip_unstable, internal_prefixes = args.internal_prefixes)
    # register rules before enabling/disabling
    register_rules(driver)
    for p in args.add_rules:
        load_module(driver, os.path.abspath(p))

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
