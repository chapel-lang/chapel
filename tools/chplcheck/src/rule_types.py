#
# Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

import typing

import chapel
from abc import ABCMeta, abstractmethod
from dataclasses import dataclass
from fixits import Fixit, Edit
from config import RuleSetting


def _build_ignore_fixit(
    anchor: chapel.AstNode, lines: typing.List[str], rule_name: str
) -> Fixit:
    # TODO: how should this handle multiple ignores?
    loc = anchor.location()
    text = chapel.range_to_text(loc, lines)
    indent_amount = max(loc.start()[1] - 1, 0)
    indent = " " * indent_amount
    text = f'@chplcheck.ignore("{rule_name}")\n' + indent + text
    ignore = Fixit.build(Edit.build(loc, text))
    ignore.description = "Ignore this warning"
    return ignore


def _get_location(node: chapel.AstNode):
    """Helper to get the location of a node"""
    if isinstance(node, chapel.NamedDecl):
        return node.name_location()
    else:
        return node.location()


@dataclass
class RuleLocation:
    path_: str
    start_: typing.Tuple[int, int]
    end_: typing.Tuple[int, int]

    def path(self) -> str:
        return self.path_

    def start(self) -> typing.Tuple[int, int]:
        return self.start_

    def end(self) -> typing.Tuple[int, int]:
        return self.end_

    @classmethod
    def from_chapel(cls, location: chapel.Location) -> "RuleLocation":
        return RuleLocation(location.path(), location.start(), location.end())


class BasicRuleResult:
    """
    Result type for basic rules. Rules can also return a plain boolean to
    represent a simple pass/fail result, with no fixit.
    """

    def __init__(
        self,
        node: chapel.AstNode,
        ignorable: bool = False,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.node = node
        self.ignorable = ignorable
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def add_fixits(self, *fixits: Fixit):
        """
        Add fixits to the result.
        """
        self._fixits.extend(fixits)

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.ignorable:
            lines = chapel.get_file_lines(context, self.node)
            ignore = _build_ignore_fixit(self.node, lines, name)
            to_return.append(ignore)
        return to_return


_BasicRuleResult = typing.Union[bool, BasicRuleResult]
"""The type of values that can be returned by basic rule functions"""


BasicRuleCheck = typing.Callable[
    [chapel.Context, chapel.AstNode], _BasicRuleResult
]
"""Function type for basic rules; (context, node) -> _BasicRuleResult"""


class AdvancedRuleResult:
    """
    Result type for advanced rules. Advanced rules can also return a plain
    boolean to represent a simple pass/fail result, with no fixit. Having an anchor implies that it is ignorable
    """

    def __init__(
        self,
        node: chapel.AstNode,
        anchor: typing.Optional[chapel.AstNode] = None,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.node = node
        self.anchor = anchor
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.anchor is not None:
            lines = chapel.get_file_lines(context, self.anchor)
            ignore = _build_ignore_fixit(self.anchor, lines, name)
            to_return.append(ignore)
        return to_return


_AdvancedRuleResult = typing.Iterator[
    typing.Union[chapel.AstNode, AdvancedRuleResult]
]
"""Internal type for advanced rule results"""


AdvancedRuleCheck = typing.Callable[
    [chapel.Context, chapel.AstNode], _AdvancedRuleResult
]
"""Function type for advanced rules"""


class LocationRuleResult:
    """
    Result type for location based rules. Rules can also return a plain RuleLocation
    """

    def __init__(
        self,
        location: RuleLocation,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.location = location
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def add_fixits(self, *fixits: Fixit):
        """
        Add fixits to the result.
        """
        self._fixits.extend(fixits)

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result.
        """
        to_return = self._fixits
        return to_return


_LocationRuleResult = typing.Iterator[
    typing.Union[RuleLocation, LocationRuleResult]
]
"""Internal type for location rule results"""


LocationRuleCheck = typing.Callable[
    [chapel.Context, str, typing.List[str]], _LocationRuleResult
]
"""Function type for location rules"""


RuleResult = typing.Union[
    _BasicRuleResult, _AdvancedRuleResult, _LocationRuleResult
]
"""Union type for all rule results"""

CheckResult = typing.Tuple[
    RuleLocation, typing.Optional[chapel.AstNode], str, typing.List[Fixit]
]


VarResultType = typing.TypeVar("VarResultType")

FixitHook = typing.Callable[
    [chapel.Context, VarResultType],
    typing.Optional[typing.Union[Fixit, typing.List[Fixit]]],
]
"""
Function type for fixits; (context, data) -> None or Fixit or List[Fixit]
"""


class Rule(typing.Generic[VarResultType], metaclass=ABCMeta):
    # can't specify type of driver due to circular import
    def __init__(self, driver, name: str, settings: typing.List[str]) -> None:
        self.driver = driver
        self.name = name
        self.settings = self._parse_settings(settings)
        self.fixit_funcs: typing.List[FixitHook[VarResultType]] = []

    def _parse_settings(
        self, settings_from_user: typing.List[str]
    ) -> typing.List[RuleSetting]:
        settings: typing.List[RuleSetting] = []
        for s in settings_from_user:
            if s.startswith("."):
                settings.append(RuleSetting(s[1:], self.name))
            else:
                settings.append(RuleSetting(s))
        return settings

    def _fixup_description_for_fixit(
        self, fixit: Fixit, fixit_func: FixitHook
    ) -> None:
        if fixit.description is not None:
            return
        if fixit_func.__doc__ is not None:
            fixit.description = fixit_func.__doc__.strip()

    def run_fixit_hooks(
        self, context: chapel.Context, result: VarResultType
    ) -> typing.List[Fixit]:
        fixits_from_hooks = []
        for fixit_func in self.fixit_funcs:
            extra_fixes = fixit_func(context, result)
            if extra_fixes is None:
                continue

            if isinstance(extra_fixes, Fixit):
                extra_fixes = [extra_fixes]

            for f in extra_fixes:
                self._fixup_description_for_fixit(f, fixit_func)
                fixits_from_hooks.append(f)
        return fixits_from_hooks

    def get_settings_kwargs(self):
        """
        Returns a Dict of SettingName -> SettingValue for this rule
        """

        # find all relevant settings for this rule
        # either they have no specific rule name, or they match this rule
        setting_kwargs = {}
        for setting in self.settings:
            # if setting is in the driver's settings, use that value
            # otherwise, default to None
            driver_setting = self.driver.config.rule_settings.get(setting)
            setting_kwargs[setting.setting_name] = driver_setting

        return setting_kwargs

    @abstractmethod
    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        pass


class BasicRule(Rule[BasicRuleResult]):
    """
    Class containing all information for the driver about basic rules
    """

    def __init__(
        self,
        driver,
        name: str,
        pattern: typing.Any,
        check_func: BasicRuleCheck,
        settings: typing.List[str],
    ) -> None:
        super().__init__(driver, name, settings)
        self.pattern = pattern
        self.check_func = check_func

    def _check_single(
        self, context: chapel.Context, node: chapel.AstNode
    ) -> typing.Optional[CheckResult]:
        setting_kwargs = self.get_settings_kwargs()
        result = self.check_func(context, node, **setting_kwargs)
        check, fixits = None, []

        # unwrap the result from the check function, and wrap it back
        # into a BasicRuleResult so we can feed it to the fixit hooks
        if isinstance(result, BasicRuleResult):
            check = False
            fixits = result.fixits(context, self.name)
        else:
            check = result
            result = BasicRuleResult(node)

        if check:
            return None

        # if we are going to warn, check for fixits from fixit hooks (in
        # addition to the fixits in the result itself)
        # add the fixits from the hooks to the fixits from the rule
        fixits = self.run_fixit_hooks(context, result) + fixits
        loc = RuleLocation.from_chapel(_get_location(result.node))
        return (loc, result.node, self.name, fixits)

    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        for node, _ in self.driver.each_matching(root, self.pattern):
            if not self.driver.should_check_rule(self.name, node):
                continue

            checked = self._check_single(context, node)
            if checked is not None:
                yield checked


class AdvancedRule(Rule[AdvancedRuleResult]):
    """
    Class containing all information for the driver about advanced
    """

    def __init__(
        self,
        driver,
        name: str,
        check_func: AdvancedRuleCheck,
        settings: typing.List[str],
    ) -> None:
        super().__init__(driver, name, settings)
        self.check_func = check_func

    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        setting_kwargs = self.get_settings_kwargs()
        for result in self.check_func(context, root, **setting_kwargs):
            if isinstance(result, AdvancedRuleResult):
                node, anchor = result.node, result.anchor
                fixits = result.fixits(context, self.name)
                if anchor is not None and not self.driver.should_check_rule(
                    self.name, anchor
                ):
                    continue
            else:
                node = result
                fixits = []
                result = AdvancedRuleResult(node)

            # For advanced rules, the traversal of the AST is out of our hands,
            # so we can't stop it from going into unstable modules. Instead,
            # once the rule emits a warning, check by traversing the AST
            # if the warning target should be skipped.
            if self.driver.config.skip_unstable and chapel.in_unstable_module(
                node
            ):
                continue

            # if we are going to warn, check for fixits from fixit hooks (in
            # addition to the fixits in the result itself)
            # add the fixits from the hooks to the fixits from the rule
            fixits = self.run_fixit_hooks(context, result) + fixits
            loc = RuleLocation.from_chapel(_get_location(node))
            yield (loc, node, self.name, fixits)


class LocationRule(Rule[LocationRuleResult]):
    """
    Class containing all information for the driver about advanced
    """

    def __init__(
        self,
        driver,
        name: str,
        check_func: LocationRuleCheck,
        settings: typing.List[str],
    ) -> None:
        super().__init__(driver, name, settings)
        self.check_func = check_func

    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        if isinstance(root, chapel.Comment):
            # TODO: we have no way to get the location of a comment to
            # determine path/lines
            yield from []
            return
        path = root.location().path()
        lines = chapel.get_file_lines(context, root)

        setting_kwargs = self.get_settings_kwargs()
        for result in self.check_func(context, path, lines, **setting_kwargs):
            if isinstance(result, LocationRuleResult):
                fixits = result.fixits(context, self.name)
            else:
                fixits = []
                result = LocationRuleResult(result)

            # if we are going to warn, check for fixits from fixit hooks (in
            # addition to the fixits in the result itself)
            # add the fixits from the hooks to the fixits from the rule
            fixits = self.run_fixit_hooks(context, result) + fixits
            loc = result.location
            yield (loc, None, self.name, fixits)
