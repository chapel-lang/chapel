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
    add_rules: List[str]

    @classmethod
    def add_arguments(cls, parser: argparse.ArgumentParser, prefix: str = ""):
        parser.add_argument(
            f"--{prefix}disable-rule",
            action="append",
            dest="chplcheck_disabled_rules",
            default=[],
            help="Disable a lint rule by name"
        )
        parser.add_argument(
            f"--{prefix}enable-rule",
            action="append",
            dest="chplcheck_enabled_rules",
            default=[],
            help="Enable a lint rule by name"
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
            f"--{prefix}add-rules",
            action="append",
            dest="chplcheck_add_rules",
            default=[],
            help="Add a custom rule file for chplcheck",
        )

    @classmethod
    def from_args(cls, args: Union[argparse.Namespace, Dict[str, Any]]):
        if isinstance(args, argparse.Namespace):
            return cls(
                disabled_rules=args.chplcheck_disabled_rules,
                enabled_rules=args.chplcheck_enabled_rules,
                skip_unstable=args.chplcheck_skip_unstable,
                internal_prefixes=args.chplcheck_internal_prefixes,
                add_rules=args.chplcheck_add_rules,
            )
        else:
            return cls(
                disabled_rules=args["chplcheck_disabled_rules"],
                enabled_rules=args["chplcheck_enabled_rules"],
                skip_unstable=args["chplcheck_skip_unstable"],
                internal_prefixes=args["chplcheck_internal_prefixes"],
                add_rules=args["chplcheck_add_rules"],
            )
