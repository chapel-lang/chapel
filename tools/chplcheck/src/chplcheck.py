#
# Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

import configargparse
from collections import defaultdict
import importlib.util
import os
import sys
import glob
import itertools
import functools
from typing import List, Optional

import chapel
from driver import LintDriver
from rules import rules
from fixits import Edit
from rule_types import CheckResult, RuleLocation
from config import Config


def print_violation(loc: RuleLocation, name: str):
    first_line, _ = loc.start()
    print("{}:{}: node violates rule {}".format(loc.path(), first_line, name))


def load_module(driver: LintDriver, file_path: str):
    """
    Load a module from a file path.
    This is used to extend the linter with custom rules.
    """
    module_name = os.path.basename(file_path).removesuffix(".py")

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
        raise ValueError(
            f"Could not find rule function '{rule_func_name}' in {file_path}"
        )
    rule_func(driver)


def int_to_nth(n: int) -> str:
    d = {
        1: "first",
        2: "second",
        3: "third",
        4: "fourth",
        5: "fifth",
        6: "sixth",
        7: "seventh",
        8: "eighth",
        9: "ninth",
        10: "tenth",
    }
    return d.get(n, f"{n}th")


def apply_fixits(
    violations: List[CheckResult],
    suffix: Optional[str],
    interactive: bool,
) -> List[CheckResult]:
    """
    Apply fixits to the Chapel source code based on user input
    Any violations not applied are returned
    """
    not_applied = []
    edits_to_apply = []
    for loc, node, rule, fixits in violations:
        if fixits is None or len(fixits) == 0:
            # no fixits to apply, skip
            not_applied.append((loc, node, rule, []))
            continue
        if not interactive:
            # apply the first fixit
            edits_to_apply.extend(fixits[0].edits)
            continue

        options = ["Skip"]
        for idx, fixit in enumerate(fixits, start=1):
            if fixit.description is not None:
                options.append(f"{fixit.description}")
            else:
                s = int_to_nth(idx)
                options.append(f"Apply {s} Fix")
        print_violation(loc, rule)
        for i, opt in enumerate(options):
            print(f"  {i}. {opt}")
        done = False
        while not done:
            try:
                choice = input("Choose an option: ")
                if choice == "0":
                    not_applied.append((loc, node, rule, fixits))
                    done = True
                else:
                    try:
                        fixit = fixits[int(choice) - 1]
                        edits_to_apply.extend(fixit.edits)
                        done = True
                    except (ValueError, IndexError):
                        print(
                            "Please enter a number corresponding to an option"
                        )
            except KeyboardInterrupt:
                # apply no edits, return the original violations
                return violations

    apply_edits(edits_to_apply, suffix)
    return not_applied


def apply_edits(edits: List[Edit], suffix: Optional[str]):
    """
    Apply a list of fixits
    """
    edits_per_file = defaultdict(lambda: [])
    for edit in edits:
        edits_per_file[edit.path].append(edit)

    # Apply edits in reverse order to avoid invalidating the locations of
    # subsequent edits
    for file, edits in edits_per_file.items():
        sorted_edits = sorted(edits, key=lambda f: f.start, reverse=True)
        with open(file, "r") as f:
            lines = f.readlines()

        prev_start = None
        for edit in sorted_edits:
            line_start, char_start = edit.start
            line_end, char_end = edit.end

            # Skip overlapping fixits
            if prev_start is not None and edit.end > prev_start:
                continue

            lines[line_start - 1] = (
                lines[line_start - 1][: char_start - 1]
                + edit.text
                + lines[line_end - 1][char_end - 1 :]
            )
            if line_start != line_end:
                lines[line_start:line_end] = [""] * (line_end - line_start)

            prev_start = edit.start

        outfile = file if suffix is None else file + suffix
        with open(outfile, "w") as f:
            f.writelines(lines)


def print_rules(driver: LintDriver, show_all=True):
    padding = max(len(rule) for (rule, _) in driver.rules_and_descriptions())
    for rule, description in driver.rules_and_descriptions():
        if description is None:
            description = ""
        # Only show the first line of the description
        description = description.strip().split("\n")[0].strip()

        prefix = ""
        if rule not in driver.SilencedRules:
            if show_all:
                prefix = "* "
        else:
            if show_all:
                prefix = "  "
            else:
                continue

        print(f"  {prefix}{rule.ljust(padding)}   {description}")


def main():
    parser = configargparse.ArgParser(
        "chplcheck",
        default_config_files=[
            os.path.join(os.getcwd(), "chplcheck.cfg"),
            os.path.join(os.getcwd(), ".chplcheck.cfg"),
            os.path.join(os.getcwd(), "Mason.toml"),
        ],
        config_file_parser_class=configargparse.CompositeConfigParser(
            [
                configargparse.YAMLConfigFileParser,
                configargparse.TomlConfigParser(["tool.chplcheck"]),
            ]
        ),
        args_for_setting_config_path=["--config", "-c"],
    )
    parser.add_argument(
        "--version",
        action="version",
        version=f"chplcheck {chapel.Context().get_compiler_version()}",
    )
    parser.add_argument("filenames", nargs="*")
    parser.add_argument(
        "--file",
        action="append",
        default=[],
        help="Add a file to the list of 'filenames' to lint",
    )
    Config.add_arguments(parser)
    parser.add_argument("--lsp", action="store_true", default=False)
    parser.add_argument(
        "--list-rules",
        action="store_true",
        default=False,
        help="List all available rules",
    )
    parser.add_argument(
        "--list-active-rules",
        action="store_true",
        default=False,
        help="List all currently enabled rules",
    )
    parser.add_argument(
        "--fixit",
        action="store_true",
        default=False,
        help="Apply fixits for the relevant rules",
    )
    parser.add_argument(
        "--fixit-suffix",
        default=None,
        help="Suffix to append to the original file name when applying fixits. If not set (the default), the original file will be overwritten.",
    )
    parser.add_argument(
        "--interactive",
        "-i",
        action="store_true",
        default=False,
        help="Apply fixits interactively, requires --fixit",
    )
    args = parser.parse_args()
    args.filenames.extend(args.file)
    args.filenames = itertools.chain(
        *map(functools.partial(glob.glob, recursive=True), args.filenames)
    )

    config = Config.from_args(args)
    driver = LintDriver(config)

    # register rules before enabling/disabling
    rules(driver)
    for p in config.add_rules:
        load_module(driver, os.path.abspath(p))

    if args.list_rules:
        print("Available rules (default rules marked with *):")
        print_rules(driver)
        return 0

    driver.disable_rules(*config.disabled_rules)
    driver.enable_rules(*config.enabled_rules)

    if args.list_active_rules:
        print("Active rules:")
        print_rules(driver, show_all=False)
        return 0

    invalid_settings = driver.validate_rule_settings()
    if invalid_settings:
        print("Invalid rule settings:")
        for r in invalid_settings:
            print(f"  {r}")
        return 2

    if args.lsp:
        from lsp import run_lsp

        run_lsp(driver)
        return 0

    printed_warning = False

    for filename, context in chapel.files_with_stdlib_contexts(args.filenames):
        # Silence errors, warnings etc. -- we're just linting.
        with context.track_errors() as _:
            asts = context.parse(filename)
            violations = list(driver.run_checks(context, asts))

            # sort the failures in order of appearance
            violations.sort(key=lambda f: f[0].start()[0])

            if args.fixit:
                violations = apply_fixits(
                    violations, args.fixit_suffix, args.interactive
                )

            for loc, _, rule, _ in violations:
                print_violation(loc, rule)
                printed_warning = True

    if printed_warning:
        return 1


if __name__ == "__main__":
    sys.exit(main())
