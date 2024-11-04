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

from dataclasses import dataclass
from typing import List, Union, Dict, Any
import argparse


@dataclass
class Config:
    """
    Configuration options that are specific to linting, not the CLI
    """

    disabled_rules: List[str]
    enabled_rules: List[str]
    skip_unstable: bool
    internal_prefixes: List[str]
    check_internal_prefixes: bool
    add_rules: List[str]

    @classmethod
    def add_arguments(cls, parser: argparse.ArgumentParser, prefix: str = ""):
        parser.add_argument(
            f"--{prefix}disable-rule",
            action="append",
            dest="chplcheck_disabled_rules",
            default=[],
            help="Disable a lint rule by name",
        )
        parser.add_argument(
            f"--{prefix}enable-rule",
            action="append",
            dest="chplcheck_enabled_rules",
            default=[],
            help="Enable a lint rule by name",
        )
        parser.add_argument(
            f"--{prefix}skip-unstable",
            action="store_true",
            dest="chplcheck_skip_unstable",
            default=False,
            help="Skip unstable rules when linting",
        )
        parser.add_argument(
            f"--{prefix}internal-prefix",
            action="append",
            dest="chplcheck_internal_prefixes",
            default=[],
            help="Add a prefix to the list of internal prefixes used when linting",
        )
        parser.add_argument(
            f"--{prefix}check-internal-prefix",
            action="append",
            dest="chplcheck_check_internal_prefixes",
            default=False,
            help="Check symbols with internal prefixes when linting",
        )
        parser.add_argument(
            f"--{prefix}add-rules",
            action="append",
            dest="chplcheck_add_rules",
            default=[],
            help="Add a custom rule file for chplcheck",
        )

    @classmethod
    def from_args(cls, args: Union[argparse.Namespace, Dict[str, Any]]):
        args = vars(args) if isinstance(args, argparse.Namespace) else args
        return cls(
            disabled_rules=args["chplcheck_disabled_rules"],
            enabled_rules=args["chplcheck_enabled_rules"],
            skip_unstable=args["chplcheck_skip_unstable"],
            internal_prefixes=args["chplcheck_internal_prefixes"],
            check_internal_prefixes=args["chplcheck_check_internal_prefixes"],
            add_rules=args["chplcheck_add_rules"],
        )
