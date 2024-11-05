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

import functools
import itertools
from typing import Any, Callable, Iterator, List, Optional, Tuple, Union

import chapel
from fixits import Fixit
import rule_types
from config import Config

IgnoreAttr = ("chplcheck.ignore", ["rule", "comment"])


def ignores_rule(node: chapel.AstNode, rulename: str):
    """
    Given an AST node, check if it has an attribute telling it to silence
    warnings for a given rule.
    """

    ag = node.attribute_group()

    if ag is None:
        return False
    for attr in ag:
        attr_call = chapel.parse_attribute(attr, IgnoreAttr)
        if attr_call is None:
            continue

        ignored_rule = attr_call["rule"]
        if ignored_rule is not None and ignored_rule.value() == rulename:
            return True

    return False


class LintDriver:
    """
    Driver class containing the state and methods for linting. Among other
    things, contains the rules for emitting warnings, as well as the
    list of rules that should be silenced.

    Provides the @driver.basic_rule and @driver.advanced_rule decorators
    for registering new rules.
    """

    def __init__(self, config: Config):
        self.config: Config = config
        self.SilencedRules: List[str] = []
        self.BasicRules: List[rule_types.BasicRule] = []
        self.AdvancedRules: List[rule_types.AdvancedRule] = []
        self.LocationRules: List[rule_types.LocationRule] = []

    def rules_and_descriptions(self):
        # Use a dict in case a rule is registered multiple times.
        to_return = {}

        for rule in itertools.chain(
            self.BasicRules, self.AdvancedRules, self.LocationRules
        ):
            doc = rule.check_func.__doc__ or ""

            # if there is an escaped underscore, remove the escape
            doc = doc.replace("\\_", "_")

            to_return[rule.name] = doc

        to_return = list(to_return.items())
        to_return.sort()
        return to_return

    def disable_rules(self, *rules: str):
        """
        Tell the driver to silence / skip warning for the given rules.
        """

        self.SilencedRules.extend(rules)

    def enable_rules(self, *rules: str):
        """
        Tell the driver to warn for the given rules even if they were
        previously disabled.
        """

        self.SilencedRules = list(set(self.SilencedRules) - set(rules))

    def should_check_rule(
        self, rulename: str, node: Optional[chapel.AstNode] = None
    ):
        if rulename in self.SilencedRules:
            return False

        if node is not None and ignores_rule(node, rulename):
            return False

        return True

    def has_internal_prefix(self, node: chapel.AstNode) -> bool:
        """
        Check if a node has a name that starts with one of the internal prefixes.
        """
        if not hasattr(node, "name"):
            return False
        return any(
            node.name().startswith(p) for p in self.config.internal_prefixes
        )

    def _preorder_skip_unstable_modules(self, node):
        if not self.config.skip_unstable:
            yield from chapel.preorder(node)
            return

        def recurse(node):
            if chapel.is_unstable_module(node):
                return

            yield node
            for child in node:
                yield from recurse(child)

        yield from recurse(node)

    def each_matching(self, node: chapel.AstNode, pattern: Any):
        yield from chapel.each_matching(
            node, pattern, iterator=self._preorder_skip_unstable_modules
        )

    def _check_rule(
        self,
        context: chapel.Context,
        root: chapel.AstNode,
        rule: Union[
            rule_types.BasicRule,
            rule_types.AdvancedRule,
            rule_types.LocationRule,
        ],
    ) -> Iterator[rule_types.CheckResult]:

        # If we should ignore the rule no matter the node, no reason to run
        # a traversal and match the pattern.
        if not self.should_check_rule(rule.name):
            return

        yield from rule.check(context, root)

    def basic_rule(self, pat, default: bool = True, settings: List[str] = []):
        """
        This method is a decorator factory for adding 'basic' rules to the
        driver. A basic rule is a function returning a boolean that gets called
        on any node that matches a pattern. If the function returns 'True', the
        node is good, and no warning is emitted. However, if the function returns
        'False', the node violates the rule.

        The name of the decorated function is used as the name of the rule.
        """

        def decorator_basic_rule(func):
            self.BasicRules.append(
                rule_types.BasicRule(
                    driver=self,
                    name=func.__name__,
                    pattern=pat,
                    check_func=func,
                    settings=settings,
                )
            )
            if not default:
                self.SilencedRules.append(func.__name__)

            @functools.wraps(func)
            def wrapper_basic_rule(*args, **kwargs):
                return func(*args, **kwargs)

            return wrapper_basic_rule

        return decorator_basic_rule

    def fixit(self, checkfunc):
        """
        Declare a fixit hook for a given rule. The hook function receives as
        parameters the Dyno context object and the AdvancedRuleResult or
        BasicRuleResult object that represents the rule violation.

        The hook function should return a list of Fixit objects which will be
        suggested to the user.
        """

        def decorator_fixit(func):
            found = False
            for rule in itertools.chain(
                self.BasicRules, self.AdvancedRules, self.LocationRules
            ):
                if rule.name == checkfunc.__name__:
                    rule.fixit_funcs.append(func)
                    found = True

            if not found:
                raise ValueError(
                    "Couldn't find rule {} to attach fixit {} to".format(
                        checkfunc.__name__, func.__name__
                    )
                )

            @functools.wraps(func)
            def wrapper_basic_rule(*args, **kwargs):
                return func(*args, **kwargs)

            return wrapper_basic_rule

        return decorator_fixit

    def advanced_rule(
        self, _func=None, *, default=True, settings: List[str] = []
    ):
        """
        This method is a decorator for adding 'advanced' rules to the driver.
        An advanced rule is a function that gets called on a root AST node,
        and is expected to traverse that AST to find places where warnings
        need to be emitted.

        Advanced rules should yield either the node to be warned for, or
        a tuple of (node, anchor). The anchor is checked for silencing,
        making it possible to support @chplcheck.ignore for the advanced rule.

        The name of the decorated function is used as the name of the rule.
        """

        def decorator_advanced_rule(func):
            self.AdvancedRules.append(
                rule_types.AdvancedRule(
                    driver=self,
                    name=func.__name__,
                    check_func=func,
                    settings=settings,
                )
            )
            if not default:
                self.SilencedRules.append(func.__name__)

            @functools.wraps(func)
            def wrapper_advanced_rule(*args, **kwargs):
                return func(*args, **kwargs)

            return wrapper_advanced_rule

        # this allows the usage of either `@advanced_rule` or `@advanced_rule()`
        if _func is None:
            return decorator_advanced_rule
        else:
            return decorator_advanced_rule(_func)

    def location_rule(
        self, _func=None, *, default=True, settings: List[str] = []
    ):
        """
        This method is a decorator for adding location-only based rules to the
        driver. A location rule is a function that gets called on a filepath
        and list of source lines, and is expected to look for places where
        warnings need to be emitted.

        The name of the decorated function is used as the name of the rule.
        """

        def decorator_location_rule(func):
            self.LocationRules.append(
                rule_types.LocationRule(
                    driver=self,
                    name=func.__name__,
                    check_func=func,
                    settings=settings,
                )
            )
            if not default:
                self.SilencedRules.append(func.__name__)

            @functools.wraps(func)
            def wrapper_location_rule(*args, **kwargs):
                return func(*args, **kwargs)

            return wrapper_location_rule

        # this allows the usage of either `@location_rule` or `@location_rule()`
        if _func is None:
            return decorator_location_rule
        else:
            return decorator_location_rule(_func)

    def validate_rule_settings(self) -> List[str]:
        """
        Validate that all rule settings are valid. Checks if user has specified a
        setting that is not recognized by any rule.

        Returns a list of unrecognized settings.
        """

        all_rule_settings = set()
        for rule in itertools.chain(
            self.BasicRules, self.AdvancedRules, self.LocationRules
        ):
            all_rule_settings.update(rule.settings)

        unrecognized_settings = []
        for setting in self.config.rule_settings:
            if setting not in all_rule_settings:
                unrecognized_settings.append(setting)

        return unrecognized_settings

    def run_checks(
        self, context: chapel.Context, asts: List[chapel.AstNode]
    ) -> Iterator[rule_types.CheckResult]:
        """
        Runs all the rules registered with this node, yielding warnings for
        all non-silenced rules that are violated in the given ASTs.
        """

        for ast in asts:
            for rule in itertools.chain(
                self.BasicRules, self.AdvancedRules, self.LocationRules
            ):
                for toreport in self._check_rule(context, ast, rule):
                    node = toreport[1]
                    if (
                        not self.config.check_internal_prefixes
                        and node
                        and self.has_internal_prefix(node)
                    ):
                        continue

                    yield toreport
